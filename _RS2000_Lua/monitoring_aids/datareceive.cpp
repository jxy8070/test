#include "datareceive.h"
#include <QUrl>
#include "monitorblackstage.h"

DataReceive::DataReceive(QWidget *parent) :
    QDialog(parent)
{
    _connectStatus = false;

    if(_devSetMonitor == NULL)
        qDebug()<<"_devSetMonitor is NULL!";
    _socket = new QWebSocket();
    if(_socket != NULL)
        qDebug()<<"Create websocket success!";
    _time = new QTimer(this);
    connect(_socket, SIGNAL(connected()), this, SLOT(onConnect()), Qt::AutoConnection);
    connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()), Qt::AutoConnection);
    connect(_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextData(QString)), Qt::AutoConnection);
    connect(_time, SIGNAL(timeout()), this, SLOT(onReconnect()));

    _socket->open(QUrl("ws://192.168.1.38:2323"));   //连接服务器
}

void DataReceive::sendTextData(QString data)
{
    _socket->sendTextMessage(data);
}

void DataReceive::createDataRecv()
{

}

void DataReceive::onConnect()
{
//    qDebug()<<"Connect success!";
    MonitorBlackStage::instance()->sendStatus("Connect success");
    _connectStatus = true;
    _time->stop();
//    qDebug()<<"Address:"<<_socket->peerAddress(); //获取IP地址
//    qDebug()<<"prot:"<<_socket->peerPort();       //获取端口号
}

void DataReceive::onTextData(QString data)
{
//    qDebug()<<"-----------------Data----------------";
//    qDebug()<<data;
}

void DataReceive::onDisconnect()
{
//    qDebug()<<"Websocket is disconnect!";
    MonitorBlackStage::instance()->sendStatus("Disconnect");
    _connectStatus = false;
    _time->start(5000);
}

void DataReceive::onReconnect()
{
//    qDebug()<<"Wedsocket is reconnect!";
    MonitorBlackStage::instance()->sendStatus("Reconnect");
    _socket->abort();           //重置套接字
    _socket->open(QUrl("ws://192.168.1.38:2323/data"));
}
