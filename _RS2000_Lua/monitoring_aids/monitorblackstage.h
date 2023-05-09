#ifndef MONITORBLACKSTAGE_H
#define MONITORBLACKSTAGE_H

#include <QObject>
#include "configItem/configitem.h"
#include "common.h"
#include <QTimer>
#include <QTime>
#include "monitoritem.h"
#include "device/AES70device.h"
#include <QPointF>
#include "datareceive.h"
#include "protocal/packet.h"

class MonitorBlackStage : public QObject
{
    Q_OBJECT
public:
    /////////复合电平表/////////
   typedef struct {
       QString clipper1;
       QString clipper2;
       QString button1;
       QString button2;
       QString button3;
       QString button4;
   }compoundMeter;

    typedef struct {
        MonitorItem *ID;      //监控设备标识
        MonitoringView *view; //当前所在画布
        CfgType cfgType;      //监控设备
        int x;                //监控设备坐标
        int y;
        QString monitorName;  //监控设备名称
        QString devIP;        //当前选择的网络设备IP
        DevType devType;      //当前选择的网络设备类型
        QString Port;         //当前选择的port

        compoundMeter _compoundMeter;
    }Monitor_Data;



    static MonitorBlackStage *instance();
    void getSettingData(MonitorItem* item);
    void removeData(MonitorItem *id);
    void upDateOnlineDev();
    QString getStringType(CfgType type);
//    void getDevsetting(DeviceItem_Monitoring_Aids *devSettingItem);
    void getView(MonitoringView * view);
    void setID(Monitor_Data data, MonitorItem *& monitorItem, MonitoringView *view);
    void setView(MonitoringView *dataView, MonitoringView *view);
    void getOpenView();
    void updateOpenData();
    bool getSameView(int index);
    void setDetection(bool b);
    QString colorToString(QColor color);
    QString displayToQString(int style);
//    void getStatus(QString Status);

    inline void clearData() { _monitorData.clear(); }
    inline void clearOpenViewData() { _openViewList.clear(); }
    inline void clearOnlineDevStatus() { _onlineDevStatus.clear(); }
    inline void clearOnlineDevType() { _onLineDev_IP_Type.clear(); }
    inline void delViewList(int index) { _openViewList.removeAt(index); }
    inline void clearViewList() { _viewList.clear(); }
    inline QList<Monitor_Data> getDataList() { return _monitorData; }
    inline void sendStatus(QString statusText) { emit sendStatusSignal(statusText); }
    inline QList<MonitoringView *> getViewList() { return _viewList; }
    inline QList<MonitoringView *> getOpenViewList() { return _openViewList; }
    inline quint32 getViewNum() { return _viewNum; }
    inline void setViewNum(quint32 num) { _viewNum = num; }

    inline void addOnlineDev_IP_Type(QString IP, QString type) {
        if(_onLineDev_IP_Type.contains(IP)){
            _onLineDev_IP_Type.remove(IP);
        }
        _onLineDev_IP_Type.insert(IP, type);
    }
    inline QMap<QString,QString> getOnlineDevTypeList() { return _onLineDev_IP_Type; }
    inline void removeOnlineDev_IP_Type(QString IP) {
        _onLineDev_IP_Type.remove(IP);
        foreach(QString type, _onLineDev_IP_Type)
            qDebug()<<"remove:"<<type;
    }
//    QJsonObject qStringToJson(const QString& data);
signals:
    void sendSettingData(QString, QString);
    void sendCompoundMeter(QString, QString, QString, QString, QString, QString);
    void sendLiveStreamingData(QString , QString);
    void getItemXY();
    void onDownSuccess(bool);
    void sendStatusSignal(QString);
public slots:
    void getDownData();
    void getMonitorData(MonitorItem *id, MonitoringView *view, CfgType cfgType, DevType devtype = Dev_Type_Unknow, QString devIP = "0.0.0.0", QString Port = "None",
                        QString clipper1 = "None", QString clipper2 = "None", QString button1 = "None",
                        QString button2 = "None", QString button3 = "None", QString button4 = "None");
    void getCompoundMeterData(MonitorItem *id, QString clipper1, QString clipper2, QString button1, QString button2, QString button3, QString button4);
    void getTimer();

private:
    MonitorBlackStage(QObject *parent = 0);                     //私有化构造函数
    MonitorBlackStage(const MonitorBlackStage&);                //私有化拷贝构造
    MonitorBlackStage& operator =(const MonitorBlackStage&);    //私有化复制拷贝构造
    static MonitorBlackStage *_instance;
    //内嵌类--用于在进程结束的时候，通过静态对象来释放资源
    class Embedded{
    public:
        ~Embedded()
        {
            if(MonitorBlackStage::_instance)
            {
                delete MonitorBlackStage::_instance;
                MonitorBlackStage::_instance = NULL;
            }
        }
    };
    static Embedded del;

    QList<Monitor_Data> _monitorData;       //存放的数据
    QList<AES70Device*> _onlineDevList;     //在线设备
    QList<MonitoringView *> _viewList;      //记录打开的画布
    QList<MonitoringView *> _openViewList;  //双击打开时所要加载的画布
    QList<Monitor_Data> _OpenDataList;      //打开画布时，加载的数据
    QMap<QString,QString> _onLineDev_IP_Type;   //保存画布中网络设备的类型
//    DeviceItem_Monitoring_Aids *_devSettingItem;

    DataReceive *_dataRecv;                 //WebSokect
//    DetactionOnlineDev *_detectionOnline;   //检测当前 在线的网络设备

    QTimer *_timer;
    QMap<QString,int> _onlineDevStatus;    //保存画布中网络设备的在线状态
    quint32 _viewNum;
//    QString _status;
};

#endif // MONITORBLACKSTAGE_H
