#ifndef DATARECEIVE_H
#define DATARECEIVE_H

#include <QObject>
#include <QWebSocket>
#include <qwebsocket.h>
#include <QString>
#include <QTimer>
#include "deviceSetting/devsetting_monitoring_aids.h"
//#include "monitorblackstage.h"

class DataReceive : public QDialog
{
    Q_OBJECT
public:
    explicit DataReceive(QWidget *parent = 0);

    void sendTextData(QString data);
signals:

public slots:
    void createDataRecv();          //创建websocket连接
    void onConnect();               //socket连接成功后，触发该函数
    void onTextData(QString data);  //收到sev端数据的时候，触发该函数
    void onDisconnect();            //断开连接的时候，触发该函数
    void onReconnect();               //重连函数

private:
    QWebSocket *_socket;             //websocket套接字
    QTimer *_time;                   //周期重连timer
    bool _connectStatus;             //websocket连接状态
    DevSetting_Monitoring_Aids *_devSetMonitor;


};

#endif // DATARECEIVE_H
