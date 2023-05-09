#include "monitorblackstage.h"
//#include <qthread.h>
#include <QFileDialog>
#include <QCoreApplication>
#include "device/devicemanager.h"
#include "device/devicemodel.h"
#include <QJsonDocument>
#include "frame/monitoringview.h"


MonitorBlackStage* MonitorBlackStage::_instance;

MonitorBlackStage::Embedded MonitorBlackStage::del;//程序结束时，系统调用其析构函数，完成资源释放
//单例模式
MonitorBlackStage *MonitorBlackStage::instance()
{
    if(_instance == NULL)
    {
       _instance = new MonitorBlackStage();
    }
    return _instance;
}


void MonitorBlackStage::getSettingData(MonitorItem *item)
{
    foreach(Monitor_Data Item, _monitorData)
    {
        if(Item.ID == item && Item.devIP != "0.0.0.0")
        {
            emit sendSettingData(Item.devIP, Item.Port);
            if(Item.cfgType == CFG_TYPE_1 && (item->DisplayStyle() == 2
              || item->DisplayStyle() == 3 || item->DisplayStyle() == 4))
            {
                QString clipper1 = Item._compoundMeter.clipper1;
                QString clipper2 = Item._compoundMeter.clipper2;
                QString button1  = Item._compoundMeter.button1;
                QString button2  = Item._compoundMeter.button2;
                QString button3  = Item._compoundMeter.button3;
                QString button4  = Item._compoundMeter.button4;
                qDebug()<<__FUNCTION__<<__LINE__<<clipper1<<clipper2<<button1<<button2<<button3<<button4;
                emit sendCompoundMeter(clipper1, clipper2, button1, button2, button3, button4);
            }
            if(Item.cfgType == CFG_TYPE_5)
            {
                QString ip = Item.devIP;
                QString videoID = Item.ID->getVideoID();
                qDebug()<<__FUNCTION__<<__LINE__<<"+++++++++++++++++++++++++++++++"<<ip<<videoID;
                emit sendLiveStreamingData(ip, videoID);
            }
        }

    }
}

void MonitorBlackStage::removeData(MonitorItem *id)
{
    for(int i = 0; i < _monitorData.count(); i++)
    {
        qDebug() << __FUNCTION__ << __LINE__
                 << _monitorData.count() << i << _monitorData.at(i).ID
                 << _monitorData.at(i).cfgType << _monitorData.at(i).devType << _monitorData.at(i).Port << id;

        if(_monitorData.at(i).ID == id)//数据存在
        {
            qDebug()<<"remonveAt:"<<i;
            _monitorData.removeAt(i);
        }
    }
}

void MonitorBlackStage::upDateOnlineDev()
{
//    if(_onlineDevList == NULL)
//    {
    emit getItemXY();
    _onlineDevList.clear();
    _onlineDevList = DeviceManager::instance()->getDeviceModel()->getDeviceList();

    bool find = false;
    if(_onlineDevList.isEmpty())
    {
//        _monitorData.clear();
    }
    if(!_monitorData.isEmpty())
    {
        for(int i = 0; i < _monitorData.count(); i++)
        {
            find = false;
            for(int j = 0; j < _onlineDevList.count(); j++)
            {
                if(_monitorData.at(i).devIP == _onlineDevList.at(j)->getDeviceIp())
                {
                    find = true;
                    break;
                }
            }
            if(!find && _monitorData.at(i).cfgType != CFG_TYPE_4 && _monitorData.at(i).cfgType != CFG_TYPE_5)
            {
                qDebug()<<__FUNCTION__<<__LINE__<<"removeAt"<<i;
                _monitorData.removeAt(i);
            }

            //获得设备坐标
//            qDebug()<<")))))))))))))))))))))))))))))))))"<<_monitorData.at(i).ID->getMonitorScenePos();
//            qDebug()<<"((((((((((((((((((((((((((((((((("<<_monitorData.at(i).ID->getTypeName();
//            int x = _monitorData.at(i).ID->getMonitorScenePos().x();
//            int y = _monitorData.at(i).ID->getMonitorScenePos().y();
            _monitorData[i].x = _monitorData.at(i).ID->getMonitorScenePos().x();
            _monitorData[i].y = _monitorData.at(i).ID->getMonitorScenePos().y();
            _monitorData[i].monitorName.clear();
            _monitorData[i].monitorName.append(_monitorData.at(i).ID->getTypeName());
            qDebug()<<__FUNCTION__<<__LINE__<<"monitorName:"<<_monitorData[i].monitorName;
        }
    }
}

QString MonitorBlackStage::getStringType(CfgType type)
{
    switch (type) {
    case CFG_TYPE_1: return "meter";
    case CFG_TYPE_2: return "clipper";
    case CFG_TYPE_3: return "button";
    case CFG_TYPE_4: return "tag";
    case CFG_TYPE_5: return "live streaming";
    case CFG_TYPE_SCENE: return "Scene";
    case CFG_TYPE_NETWORKDEVICE: return "Network device";
    default: return "UnKnow";
    }
}

//void MonitorBlackStage::getDevsetting(DeviceItem_Monitoring_Aids *devSettingItem)
//{
//    _devSettingItem = devSettingItem;
//}

void MonitorBlackStage::getView(MonitoringView *view)
{
    for(int i = 0; i < _viewList.count(); i++)
    {
        if(_viewList.at(i) == view){
            _viewList.removeAt(i);
        }
    }
//    qDebug()<<"-------------------compare";
    _viewList.append(view);
    _viewNum = _viewList.count();
    qDebug()<<"view append!";
    for(int i = 0; i < _viewList.count(); i++){
        qDebug()<<""<<&_viewList.at(i);
    }
}

void MonitorBlackStage::setID(Monitor_Data data, MonitorItem *& monitorItem, MonitoringView *view)
{
//    qDebug()<<__FUNCTION__<<__LINE__<<monitorItem;
    for(int i = 0; i < _monitorData.count(); i++){
        if(data.ID == _monitorData.at(i).ID){
            Monitor_Data monitorData;
            monitorData.ID      = monitorItem;
            monitorData.cfgType = _monitorData.at(i).cfgType;
            monitorData.devIP   = _monitorData.at(i).devIP;
            monitorData.devType = _monitorData.at(i).devType;
            monitorData.monitorName = _monitorData.at(i).monitorName;
            monitorData.Port    = _monitorData.at(i).Port;
            monitorData.view    = view;
            _OpenDataList.append(monitorData);
//            _monitorData.removeAt(i);
            qDebug()<<__FUNCTION__<<__LINE__<<monitorData.ID<<"  "<<monitorData.monitorName<<" "<<monitorItem;
        }
    }
}

void MonitorBlackStage::setView(MonitoringView *dataView, MonitoringView *view)
{
//    qDebug()<<"--------------monitorData:"<<_monitorData.count();
    foreach(Monitor_Data data, _monitorData){
        qDebug()<<__FUNCTION__<<__LINE__<<data.view<<" "<<dataView;
        if(data.view == dataView){
            data.view = view;
//            qDebug()<<"data.view:"<<data.view;
        }
    }
}

void MonitorBlackStage::getOpenView()
{
    foreach(MonitoringView *view, _viewList){
        _openViewList.append(view);
    }
}

void MonitorBlackStage::updateOpenData()
{
    _monitorData.clear();
    foreach(Monitor_Data data, _OpenDataList){
        _monitorData.append(data);
    }
    _OpenDataList.clear();
}

bool MonitorBlackStage::getSameView(int index)
{
    qDebug()<<__FUNCTION__<<__LINE__<<&_monitorData.at(index).view<<"  "
           <<&_viewList.at(0);
    if(_monitorData.at(index).view != _openViewList.at(0))
    {
        return false;
    }
//    _viewList.removeAt(0);
    return true;
}

void MonitorBlackStage::setDetection(bool b)
{
    if(b)
    {
        if(!_timer->isActive())
            _timer->start(5000);
    }else
    {
        _timer->stop();
    }
}

QString MonitorBlackStage::colorToString(QColor color)
{
    int red   = color.red();
    int green = color.green();
    int blue  = color.blue();
    return QString("%1,%2,%3").arg(red).arg(green).arg(blue);
}

QString MonitorBlackStage::displayToQString(int style)
{
    switch (style) {
    case 1: return "A";
    case 2: return "B";
    case 3: return "C";
    case 4: return "D";
    case 5: return "E";
    default: return "NoStyle";
    }
}

//void MonitorBlackStage::getStatus(QString Status)
//{
//    _status =Status;
//}

//QJsonObject MonitorBlackStage::qStringToJson(const QString& data)
//{
//    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toLocal8Bit().data());

//    if(jsonDocument.isNull())
//        qDebug()<<"jsonDocument is NULL!"<<data.toLocal8Bit().data();
//    QJsonObject JsonObject = jsonDocument.object();
//    return JsonObject;
//}

void MonitorBlackStage::getMonitorData(MonitorItem *id, MonitoringView *view, CfgType cfgType, DevType devType,QString devIP, QString Port,
                                       QString clipper1, QString clipper2, QString button1,QString button2, QString button3, QString button4)
{
    Monitor_Data data;
    //    memset(&data, 0, sizeof(Monitor_Data));
//    if(id->getType() == CFG_TYPE_4)
//    {
//        data.devIP = "";
//        data.Port = "";
//    }
    data.ID      = id;
    data.view    = view;
    data.devIP   = devIP;
    data.cfgType = cfgType;
    data.devType = devType;
    data.Port.append(Port);

    qDebug() << __FUNCTION__  << __LINE__ <<data.monitorName<<data.ID;
   data._compoundMeter.clipper1.append(/*"clipper1/" + */clipper1);
   data._compoundMeter.clipper2.append(/*"clipper2/" + */clipper2);
   data._compoundMeter.button1.append(/*"button1/" + */button1);
   data._compoundMeter.button2.append(/*"button2/" + */button2);
   if(button3 == "None" && button4 == "None")
   {
       data._compoundMeter.button3.append(button3);
       data._compoundMeter.button4.append(button4);
   }else
   {
       data._compoundMeter.button3.append(/*"button3/" + */button3);
       data._compoundMeter.button4.append(/*"button4/" + */button4);
   }
    //    quint32 len = Port.length() + data.Port.length();
    //    data.Port.resize(len);
    //    memcpy(&data.Port, &Port, sizeof(QString));
    qDebug()<<__FILEW__<<"  monitorItem:"<<id<<" _view:"<<view<<" monitorItem->getType:"<<cfgType<<" devType:"<<devType<<" devIP:"<<devIP<<" port:"<<Port;
    removeData(data.ID);

    _monitorData.append(data);
    for(int i = 0; i < _monitorData.count(); i++)
        qDebug()<<"save success!"
                <<" "<<"item:"   <<_monitorData.at(i).ID
                <<" "<<"ID:"     <<&_monitorData.at(i).ID
                <<" "<<"view:"   <<&_monitorData.at(i).view
                <<" "<<"cfgType:"<<_monitorData.at(i).cfgType
                <<" "<<"DevType:"<<_monitorData.at(i).devType
                <<" "<<"devIP:"  <<_monitorData.at(i).devIP
               <<" "<<"Port:"   <<_monitorData.at(i).Port;
}

void MonitorBlackStage::getCompoundMeterData(MonitorItem *id, QString clipper1, QString clipper2, QString button1, QString button2, QString button3, QString button4)
{
//    foreach(Monitor_Data item, _monitorData)
//    {
//        if(item.ID == id)
//        {
//            qDebug() << __FUNCTION__  << __LINE__ <<item.monitorName<<item.ID;
//           item._compoundMeter.clipper1.append(/*"clipper1/" + */clipper1);
//           item._compoundMeter.clipper2.append(/*"clipper2/" + */clipper2);
//           item._compoundMeter.button1.append(/*"button1/" + */button1);
//           item._compoundMeter.button2.append(/*"button2/" + */button2);
//           if(button3 == "None" && button4 == "None")
//           {
//               item._compoundMeter.button3.append(button3);
//               item._compoundMeter.button4.append(button4);
//           }else
//           {
//               item._compoundMeter.button3.append(/*"button3/" + */button3);
//               item._compoundMeter.button4.append(/*"button4/" + */button4);
//           }

//           qDebug() << __FUNCTION__ << __LINE__ << item._compoundMeter.clipper1 << " "
//                  << item._compoundMeter.clipper2 << " " << item._compoundMeter.button1 << " "
//                  << item._compoundMeter.button2 << " " << item._compoundMeter.button3 << " "
//                  << item._compoundMeter.button4;
//        }
//    }
//    foreach(Monitor_Data item, _monitorData)
//    {
//        qDebug() << __FUNCTION__ << __LINE__ << item._compoundMeter.clipper1 << " "
//               << item._compoundMeter.clipper2 << " " << item._compoundMeter.button1 << " "
//               << item._compoundMeter.button2 << " " << item._compoundMeter.button3 << " "
//               << item._compoundMeter.button4;
//    }
}

void MonitorBlackStage::getTimer()
{

}


void MonitorBlackStage::getDownData()
{
    if(_monitorData.isEmpty()){
        emit onDownSuccess(false);
        return ;
    }
    for(int i = 0; i < _monitorData.count(); i++)
    {
        if(_monitorData.at(i).devIP.isEmpty()){
            emit onDownSuccess(false);
            return ;
        }
            qDebug()<<"DownData!"
                   <<" "<<"ID:"<<_monitorData.at(i).ID
                  <<" "<<"x:" <<_monitorData.at(i).x
                 <<" "<<"y:" <<_monitorData.at(i).y
                <<" "<<"montorName:"<<_monitorData.at(i).monitorName
               <<" "<<"cfgType:"   <<_monitorData.at(i).cfgType
              <<" "<<"devType:"   <<_monitorData.at(i).devIP
             <<" "<<"Port:"      <<_monitorData.at(i).Port;
    }

    QString curPath  = QCoreApplication::applicationDirPath();
    QString dlgTitle = "保存文件";
    QString filter   = "json文件(*.json);;text文件(*.text);;h文件(*.h);;C++文件(*.c++);;所有文件(*.*)";
    QString FileName = QFileDialog::getSaveFileName(NULL, dlgTitle, curPath, filter);
    if(FileName != NULL)
    {
        qDebug()<<"save:"<<FileName;
        QFile file(FileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);


        QJsonObject obj;
        obj.insert("msgType","CONFIG INFO");

        QJsonDocument doc;
        QJsonArray tabsArray;
        int i = 0;
        foreach(MonitoringView *tabItem, _viewList){

            QJsonObject tabsObject;
            QJsonArray itemArray;
//            qDebug()<<"===============================name;"<<i++;
            tabsObject.insert("tabName",tabItem->getName());
//            tabsObject.insert("tabWidth",QString("%1").arg(tabItem->getRect().width()));
//            tabsObject.insert("tabHeight",QString("%1").arg(tabItem->getRect().height()));

            foreach (Monitor_Data item, _monitorData) {
                if(tabItem != item.view){
                    continue;
                }
                QJsonObject itemObject;


                itemObject.insert("tag", QString(item.monitorName));
                itemObject.insert("w", QString().setNum(item.ID->getMonitorWidth()));
                itemObject.insert("h", QString().setNum(item.ID->getMonitorHeight()));
                itemObject.insert("x", QString("%1").arg(item.x));
                itemObject.insert("y", QString("%1").arg(item.y));
                itemObject.insert("class", displayToQString(item.ID->DisplayStyle()));
                itemObject.insert("IP", QString(item.devIP));
                itemObject.insert("devType", item.devType);
                itemObject.insert("Port", QString(item.Port));
                if(item.cfgType == CFG_TYPE_1)
                {
                    itemObject.insert("ID", QString("M%1").arg(i++));
                    if(item.ID->DisplayStyle() == 2 || item.ID->DisplayStyle() == 3 || item.ID->DisplayStyle() == 4)
                    {
                        qDebug()<<item.monitorName << item._compoundMeter.clipper1<<" "
                                  <<item._compoundMeter.clipper2<<" "
                                      <<item._compoundMeter.button1<<" "
                                        <<item._compoundMeter.button2<<" "
                                          <<item._compoundMeter.button3<<" "
                                            <<item._compoundMeter.button4;
                        itemObject.insert("clipper1", QString("clipper%1/").arg(i++) + item._compoundMeter.clipper1);
                        itemObject.insert("clipper2", QString("clipper%1/").arg(i++) + item._compoundMeter.clipper2);
                        itemObject.insert("button1",  QString("button%1/").arg(i++) + item._compoundMeter.button1);
                        itemObject.insert("button2",  QString("button%1/").arg(i++) + item._compoundMeter.button2);
                        itemObject.insert("button3",  QString("button%1/").arg(i++) + item._compoundMeter.button3);
                        itemObject.insert("button4",  QString("button%1/").arg(i++) + item._compoundMeter.button4);
                    }
                }
                if(item.cfgType == CFG_TYPE_2)
                    itemObject.insert("ID", QString("C%1").arg(i++));
                if(item.cfgType == CFG_TYPE_3)
                {
                    itemObject.insert("ID", QString("B%1").arg(i++));
                    itemObject.insert("backgroundColor", colorToString(item.ID->getFillColor()));
                    itemObject.insert("nameTextColor", colorToString(item.ID->getTitilColor()));
                    itemObject.insert("textStyle", QString(item.ID->textFont().family()));
                    itemObject.insert("textSize", item.ID->textFont().pointSize());
                    itemObject.insert("text", item.ID->getText());
                }
                if(item.cfgType == CFG_TYPE_4)
                {
                    itemObject.insert("ID", QString("L%1").arg(i++));
                    itemObject.insert("backgroundColor", colorToString(item.ID->getFillColor()));
                    itemObject.insert("nameTextColor", colorToString(item.ID->getTitilColor()));
                    itemObject.insert("textStyle", QString(item.ID->textFont().family()));
                    itemObject.insert("textSize", item.ID->textFont().pointSize());
                }
                if(item.cfgType == CFG_TYPE_5)
                {
                    itemObject.insert("ID", QString("V%1").arg(i++));
                    itemObject.insert("videoID", item.ID->getVideoID());
                }
                if(item.cfgType == CFG_TYPE_SCENE)
                {
                    itemObject.insert("ID", QString("S%1").arg(i++));
                }
                if(item.cfgType == CFG_TYPE_NETWORKDEVICE)
                {
                    itemObject.insert("ID", QString("Dev%1").arg(i++));
                    itemObject.insert("type", getStringType(item.cfgType) + "/" + Config::curMfrsCfg().getDevTypeName(item.devType));
                }else
                {
                    itemObject.insert("type", getStringType(item.cfgType));
                }
                itemArray.append(itemObject);

                tabsObject.insert("items",itemArray);
            }
            tabsArray.append(tabsObject);
        }
        obj.insert("config",tabsArray);
        doc.setObject(obj);

        int w = file.write(doc.toJson());
        _dataRecv->sendTextData(doc.toJson());
        file.close();
//        _timer->start(40);
        if(w >= 0){
            emit onDownSuccess(true);
        }else{
            emit onDownSuccess(false);
        }
    }else{
        emit onDownSuccess(false);
    }
}

MonitorBlackStage::MonitorBlackStage(QObject *parent) : QObject(parent)
{
    qDebug()<<"Create BlackStage";
    _viewNum = 0;
    _dataRecv = new DataReceive();
//    /*DetactionOnlineDev **/_detectionOnline = new DetactionOnlineDev(this);
//    QThread *thread = new QThread();
//    _detectionOnline->moveToThread(thread);
//    thread->start();

//    _timer = new QTimer(this);
//    connect(_timer, SIGNAL(timeout()), this, SLOT(getTimer()));
    upDateOnlineDev();
//    srand(time(NULL));
}
