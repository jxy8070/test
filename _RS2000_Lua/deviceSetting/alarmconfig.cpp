#include "alarmconfig.h"
#include "protocal/packet.h"
#include "device/AES70device.h"
#include "device/devicemanager.h"
#include "frame/mainwindow.h"
#include "frame/projectview.h"
#include "frame/projectscene.h"
#include "sysdbutility.h"

QDataStream& operator<<(QDataStream& ds, LimitDef& limitDef)
{
    ds << limitDef.checkMax << limitDef.maxValue << limitDef.checkMin << limitDef.minValue;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, LimitDef& limitDef)
{
    ds >> limitDef.checkMax >> limitDef.maxValue >> limitDef.checkMin >> limitDef.minValue;
    return ds;
}


LimitDef::LimitDef()
{
    checkMax = false;
    maxValue = 0;
    checkMin = false;
    minValue = 0;
    state = StateNormal;
    curVlaue = 0;
}

void LimitDef::setLimitMode(int mode)
{
    switch(mode)
    {
    case 0:
        checkMin = false;
        checkMax = false;
        break;
    case 1:
        checkMin = true;
        checkMax = true;
        break;
    case 2:
        checkMin = true;
        checkMax = false;
        break;
    case 3:
        checkMin = false;
        checkMax = true;
        break;
    default:
        Q_ASSERT(false);
    }
}

int LimitDef::getLimitMode()
{
    int mode;
    if(checkMin == false && checkMax == false) mode = 0;
    else if(checkMin == true && checkMax == true) mode = 1;
    else if(checkMin == true && checkMax == false) mode = 2;
    else if(checkMin == false && checkMax == true) mode = 3;
    else Q_ASSERT(false);

    return mode;
}

AlarmConfig::AlarmConfig(QString deviceName, QString mac)
{
    _deviceName = deviceName;
    _voltMax = 60;
    _mac = mac;
    _devState = true;
    _dbHasThisRecord = false;
    connect(this, SIGNAL(needInsertAlarm(QString,int,QString)), Config::instance()->getAlarmDBBase(),
            SLOT(insertAlarmInfo(QString,int,QString)));
//    connect(this, SIGNAL(needInsertAlarm(int,QString)), this, SLOT(insertAlarmInfo(int,QString)));
//    loadAlarmSetting(mac);
}

void AlarmConfig::checkLimit(DevType devType, ADC_Data& adcData)
{
    Q_ASSERT(devType == Dev_Type_STM32_POE_AES67_SPK_0_2 || devType == Dev_Type_RT1052_POE_AES67_LINE_4_4
             || devType == Dev_Type_STM32_POE_AES67_SPK_0_1
             || devType == Dev_Type_AES67_SPK || devType == Dev_Type_AES67_MONITOR_SPK
             || devType == Dev_Type_RT1052_POE_AES67_MIC_4_0);
    double volt1Ch = adcData.volt1Ch/4095.0* _voltMax;
    double curr1Ch = adcData.curr1Ch/4095.0* _voltMax/4.0;
    double temp = adcData.Temperature/4095.0* 120 - 20;

    checkLimit(Volt1Ch, volt1Ch);
    checkLimit(Curr1Ch, curr1Ch);
    checkLimit(Temperature, temp);
    checkLimit(VolumeL, adcData.volumeL);

    if(devType == Dev_Type_STM32_POE_AES67_SPK_0_2)
    {
        double volt2Ch = adcData.volt2Ch/4095.0* _voltMax;
        double curr2Ch = adcData.curr2Ch/4095.0* _voltMax/4.0;
        checkLimit(Volt2Ch, volt2Ch);
        checkLimit(Curr2Ch, curr2Ch);
        checkLimit(VolumeR, adcData.volumeR);
    }
    else if (devType == Dev_Type_RT1052_POE_AES67_LINE_4_4)
    {

    }
    else if (devType == Dev_Type_RT1052_POE_AES67_MIC_4_0)
    {

    }
    else if (devType == Dev_Type_STM32_POE_AES67_SPK_0_1)
    {

    }
    else if (devType == Dev_Type_AES67_MONITOR_SPK)
    {

    }
    /* SPK的右电频表不需要报警 */
    else
    {
        _paraArray[VolumeR].curVlaue = adcData.volumeR;
    }
    //判断状态，根据需要报警
    if(_devState != (bool)adcData.AMP_Error)
    {
        _devState = adcData.AMP_Error;
        if (adcData.AMP_Error)
        {
            emit needInsertAlarm(_mac, 0, tr("Device Error"));
        } else {
            emit needInsertAlarm(_mac, 0, tr("Recovery from Device Error"));
        }
    }
}

void AlarmConfig::checkLimit(LimitParaType paraType, double value)
{
    AES70Device *device = DeviceManager::instance()->getDeviceByMac(_mac);
    QString typeName;
    int ch = 1;
    switch(paraType)
    {
    case Volt1Ch:
        typeName = tr("Voltage");
        ch = 1;
        break;
    case Volt2Ch:
        typeName = tr("Voltage");
        ch = 2;
        break;
    case Curr1Ch:
        typeName = tr("Current");
        ch = 1;
        break;
    case Curr2Ch:
        typeName = tr("Current");
        ch = 2;
        break;
    case Temperature:
        typeName = tr("Temp");
        ch = 0;
        break;
    case VolumeL:
        if(device->getDeviceType() == Dev_Type_AES67_SPK)
        {
            typeName = tr("Volume");
        }
        else
        {
            typeName = tr("Volume 1");
        }
        ch = 1;
        break;
    case VolumeR:
        typeName = tr("Volume 2");
        ch = 2;
        break;
    default:
        typeName = tr("????");
        break;
    }
    LimitDef &paraDef = _paraArray[paraType];
    paraDef.curVlaue = value;
//    if (paraType == VolumeL)
//    {
//        qDebug() << paraType
//                 << paraDef.checkMin
//                 << paraDef.checkMax
//                 << value
//                 << paraDef.minValue
//                 << paraDef.maxValue
//                 << paraDef.state;
//    }
    if (!_enable)
    {
        paraDef.state = StateNormal;
        return;
    }
    switch(paraDef.state)
    {
    case StateMin://Lower Alarm
        if(paraDef.checkMin)//需要判断下限
        {
            if (value > paraDef.minValue)
            {
                emit needInsertAlarm(_mac, ch, tr("Recovery from %1 Lower Alarm").arg(typeName));
                paraDef.state = StateNormal;  //正常
            }
        }
        else
        {
            emit needInsertAlarm(_mac, ch, tr("Closed %1 Lower Alarm").arg(typeName));
            paraDef.state = StateNormal;   //正常
        }
        break;
    case StateNormal://normal
        if(paraDef.checkMin && value < paraDef.minValue)//需要判断下限 且 value小于下限
        {
            emit needInsertAlarm(_mac, ch, tr("%1 %2 < %3").arg(typeName).arg(value).arg(paraDef.minValue));
            paraDef.state = StateMin;//超下限
        }
        else if(paraDef.checkMax && value > paraDef.maxValue)//需要判断上限  且  value大于上限
        {
            emit needInsertAlarm(_mac, ch, tr("%1 %2 > %3").arg(typeName).arg(value).arg(paraDef.maxValue));
            paraDef.state = StateMax;//超上限
        }
        break;
    case StateMax://Upper Alarm
        if(paraDef.checkMax)//需要判断上限
        {
            if (value < paraDef.maxValue)//如果value小于上限
            {
                emit needInsertAlarm(_mac, ch, tr("Recovery from %1 Upper Alarm").arg(typeName));
                paraDef.state = StateNormal;//正常
            }
        }
        else
        {
            emit needInsertAlarm(_mac, ch, tr("Closed %1 Upper Alarm").arg(typeName));
            paraDef.state = StateNormal;//正常
        }
        break;
    default:
        break;
    }
}

double AlarmConfig::getParaValue(LimitParaType paraType)
{
    return _paraArray[paraType].curVlaue;
}

void AlarmConfig::clearAlarmSetting()
{
    _enable = false;
    for (int i = 0; i < ParaCount; ++i)
    {
        _paraArray[i].checkMax = false;
        _paraArray[i].maxValue = 0;
        _paraArray[i].checkMin = false;
        _paraArray[i].minValue = 0;
        _paraArray[i].curVlaue = 0;
        _paraArray[i].state = StateNormal;
    }
}

void AlarmConfig::loadAlarmSetting()
{
    QString fileName = MainWindow::instance()->getSolutionPath("Alarm.cfg");

    if (fileName != NULL)
    {
        QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
        QString informationKey = QString(ALARM_GROUP).arg(_mac) + QString("/") +
                                 QString(ALARM_CONFIG_CH1_VOLTAGE_CHECKMAX);
        if (!ConfigIni->contains(informationKey))//如没有一个名叫informationKey的设置
        {
            clearAlarmSetting();
            delete ConfigIni;
            return;
        }

        ConfigIni->beginGroup(QString(ALARM_GROUP).arg(_mac));
        _enable = ConfigIni->value(QString(ALARM_CONFIG_ENABLE)).toBool();

        _paraArray[0].checkMax = ConfigIni->value(QString(ALARM_CONFIG_CH1_VOLTAGE_CHECKMAX)).toBool();
        _paraArray[0].maxValue = ConfigIni->value(QString(ALARM_CONFIG_CH1_VOLTAGE_MAXVALUE)).toInt();
        _paraArray[0].checkMin = ConfigIni->value(QString(ALARM_CONFIG_CH1_VOLTAGE_CHECKMIN)).toBool();
        _paraArray[0].minValue = ConfigIni->value(QString(ALARM_CONFIG_CH1_VOLTAGE_MINVALUE)).toInt();

        _paraArray[1].checkMax = ConfigIni->value(QString(ALARM_CONFIG_CH1_CURRENT_CHECKMAX)).toBool();
        _paraArray[1].maxValue = ConfigIni->value(QString(ALARM_CONFIG_CH1_CURRENT_MAXVALUE)).toInt();
        _paraArray[1].checkMin = ConfigIni->value(QString(ALARM_CONFIG_CH1_CURRENT_CHECKMIN)).toBool();
        _paraArray[1].minValue = ConfigIni->value(QString(ALARM_CONFIG_CH1_CURRENT_MINVALUE)).toInt();

        _paraArray[2].checkMax = ConfigIni->value(QString(ALARM_CONFIG_CH2_VOLTAGE_CHECKMAX)).toBool();
        _paraArray[2].maxValue = ConfigIni->value(QString(ALARM_CONFIG_CH2_VOLTAGE_MAXVALUE)).toInt();
        _paraArray[2].checkMin = ConfigIni->value(QString(ALARM_CONFIG_CH2_VOLTAGE_CHECKMIN)).toBool();
        _paraArray[2].minValue = ConfigIni->value(QString(ALARM_CONFIG_CH2_VOLTAGE_MINVALUE)).toInt();

        _paraArray[3].checkMax = ConfigIni->value(QString(ALARM_CONFIG_CH2_CURRENT_CHECKMAX)).toBool();
        _paraArray[3].maxValue = ConfigIni->value(QString(ALARM_CONFIG_CH2_CURRENT_MAXVALUE)).toInt();
        _paraArray[3].checkMin = ConfigIni->value(QString(ALARM_CONFIG_CH2_CURRENT_CHECKMIN)).toBool();
        _paraArray[3].minValue = ConfigIni->value(QString(ALARM_CONFIG_CH2_CURRENT_MINVALUE)).toInt();

        _paraArray[4].checkMax = ConfigIni->value(QString(ALARM_CONFIG_TEMPERATURE_CHECKMAX)).toBool();
        _paraArray[4].maxValue = ConfigIni->value(QString(ALARM_CONFIG_TEMPERATURE_MAXVALUE)).toInt();
        _paraArray[4].checkMin = ConfigIni->value(QString(ALARM_CONFIG_TEMPERATURE_CHECKMIN)).toBool();
        _paraArray[4].minValue = ConfigIni->value(QString(ALARM_CONFIG_TEMPERATURE_MINVALUE)).toInt();

        _paraArray[5].checkMax = ConfigIni->value(QString(ALARM_CONFIG_CH1_METER_CHECKMAX)).toBool();
        _paraArray[5].maxValue = ConfigIni->value(QString(ALARM_CONFIG_CH1_METER_MAXVALUE)).toInt();
        _paraArray[5].checkMin = ConfigIni->value(QString(ALARM_CONFIG_CH1_METER_CHECKMIN)).toBool();
        _paraArray[5].minValue = ConfigIni->value(QString(ALARM_CONFIG_CH1_METER_MINVALUE)).toInt();

        _paraArray[6].checkMax = ConfigIni->value(QString(ALARM_CONFIG_CH2_METER_CHECKMAX)).toBool();
        _paraArray[6].maxValue = ConfigIni->value(QString(ALARM_CONFIG_CH2_METER_MAXVALUE)).toInt();
        _paraArray[6].checkMin = ConfigIni->value(QString(ALARM_CONFIG_CH2_METER_CHECKMIN)).toBool();
        _paraArray[6].minValue = ConfigIni->value(QString(ALARM_CONFIG_CH2_METER_MINVALUE)).toInt();

        ConfigIni->endGroup();
        delete ConfigIni;
    }
    else
    {
        clearAlarmSetting();
    }
}

void AlarmConfig::saveAlarmSetting()
{
    QString fileName = MainWindow::instance()->getSolutionPath("Alarm.cfg");
    if (fileName != NULL)
    {
        QSettings *ConfigIni = new QSettings(fileName, QSettings::IniFormat, 0);
        ConfigIni->remove(QString(ALARM_GROUP).arg(_mac));
        ConfigIni->beginGroup(QString(ALARM_GROUP).arg(_mac));
        ConfigIni->setValue(QString(ALARM_CONFIG_ENABLE), _enable);

        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_VOLTAGE_CHECKMAX), _paraArray[0].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_VOLTAGE_MAXVALUE), _paraArray[0].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_VOLTAGE_CHECKMIN), _paraArray[0].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_VOLTAGE_MINVALUE), _paraArray[0].minValue);

        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_CURRENT_CHECKMAX), _paraArray[1].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_CURRENT_MAXVALUE), _paraArray[1].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_CURRENT_CHECKMIN), _paraArray[1].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_CURRENT_MINVALUE), _paraArray[1].minValue);

        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_VOLTAGE_CHECKMAX), _paraArray[2].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_VOLTAGE_MAXVALUE), _paraArray[2].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_VOLTAGE_CHECKMIN), _paraArray[2].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_VOLTAGE_MINVALUE), _paraArray[2].minValue);

        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_CURRENT_CHECKMAX), _paraArray[3].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_CURRENT_MAXVALUE), _paraArray[3].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_CURRENT_CHECKMIN), _paraArray[3].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_CURRENT_MINVALUE), _paraArray[3].minValue);

        ConfigIni->setValue(QString(ALARM_CONFIG_TEMPERATURE_CHECKMAX), _paraArray[4].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_TEMPERATURE_MAXVALUE), _paraArray[4].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_TEMPERATURE_CHECKMIN), _paraArray[4].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_TEMPERATURE_MINVALUE), _paraArray[4].minValue);

        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_METER_CHECKMAX), _paraArray[5].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_METER_MAXVALUE), _paraArray[5].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_METER_CHECKMIN), _paraArray[5].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH1_METER_MINVALUE), _paraArray[5].minValue);

        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_METER_CHECKMAX), _paraArray[6].checkMax);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_METER_MAXVALUE), _paraArray[6].maxValue);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_METER_CHECKMIN), _paraArray[6].checkMin);
        ConfigIni->setValue(QString(ALARM_CONFIG_CH2_METER_MINVALUE), _paraArray[6].minValue);

        ConfigIni->endGroup();
        delete ConfigIni;
    }
}
