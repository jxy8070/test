#include "ctrlitemrs485.h"
#include "ctrlsettingserial.h"

CtrlItemRs485::CtrlItemRs485(Device_Trn* trnDev, CfgType cfgType, QString name, QString typeName, QPointF scenePos, int width)
    :ConfigItem(trnDev, cfgType, name, typeName, scenePos, width)
{
    _rectHeight = 36;
    setRect(0, 0, _rectWidth, _rectHeight + _pinsHeight);

    QPointF pos(_rectWidth -PortWidth, 2);
    QString portName = QString("COM2");
    ItemPort* port = new ItemPort(PortType_RS232, portName, 1, false, pos, PORT_TEXT_LENGTH, this);
    _outPortList.append(port);
}


QWidget *CtrlItemRs485::getSettingForm()
{
    if (_setting == NULL) {
        _setting = new CtrlSettingSerial();
    }

    return _setting;
}
