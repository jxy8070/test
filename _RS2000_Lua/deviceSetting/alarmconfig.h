#ifndef ALARMCONFIG_H
#define ALARMCONFIG_H
#include <QObject>
#include <QtSql>

enum LimitState
{
    StateMin = -1,
    StateNormal = 0,
    StateMax = 1
};

struct LimitDef
{
    friend QDataStream& operator<<(QDataStream& ds, LimitDef& devItem);
    friend QDataStream& operator>>(QDataStream& ds, LimitDef& devItem);
    LimitDef();
    int getLimitMode();
    void setLimitMode(int mode);
    bool checkMax;      //是否需要判断上限
    int maxValue;       //上限
    bool checkMin;      //是否需要判断下限
    int minValue;       //下限
    LimitState state;   //-1 超下限   0 正常  1 超上限
    double curVlaue;
};

enum LimitParaType
{
    Volt1Ch = 0,
    Curr1Ch,
    Volt2Ch,
    Curr2Ch,
    Temperature,
    VolumeL,
    VolumeR,
    ParaCount
};

struct ADC_Data;
enum DevType;

class AlarmConfig : public QObject
{
    Q_OBJECT
public:
    AlarmConfig(QString deviceName, QString mac);
    void checkLimit(DevType devType, ADC_Data& value);
    double getParaValue(LimitParaType paraType);
    void setMinLimit(LimitParaType type, double value) { _paraArray[type].minValue = value;}
    void setMaxLimit(LimitParaType type, double value) { _paraArray[type].maxValue = value;}
    void setLimitMode(LimitParaType type, int mode) {_paraArray[type].setLimitMode(mode);}
    LimitDef& getLimitDef(LimitParaType type) {return _paraArray[type];}
    void clearAlarmSetting();
    void loadAlarmSetting();
    void saveAlarmSetting();
    void setAlarmEnable(bool enable) {_enable = enable;}
    bool getAlarmEnable() {return _enable;}
    void setMaxVolt(int volt) {_voltMax = volt;}
    bool getDevState() { return _devState;}
private:
//    QSqlError insertAlarmConfig();
//    QSqlError selectAlarmConfig();
//    QSqlError updateAlarmConfig();

signals:
    void needInsertAlarm(QString, int, QString);
private slots:


private:    
//    QSqlQuery _query;
    void checkLimit(LimitParaType para, double value);
    LimitDef _paraArray[ParaCount];
    int _voltMax;
    QString _deviceName;
    QString _mac;
    bool _devState; //设备状态
    bool _enable;
    bool _dbHasThisRecord;
//    static QLatin1String _insertAlarmInfo;
//    static QLatin1String _insertAlarmConfig;
//    static QLatin1String _selectAlarmConfig;
//    static QLatin1String _deleteAlarmConfig;
};


#endif // ALARMCONFIG_H
