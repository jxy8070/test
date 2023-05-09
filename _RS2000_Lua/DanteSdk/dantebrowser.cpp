#include "stable.h"
#include "dantebrowser.h"
#include "config.h"

//#define PRINT_AUDINATE_DEVICE_SEARCH_DEBUG //输出调试信息
////////////////////////////////////////////////////////////////////////////////
/// Callbacks
////////////////////////////////////////////////////////////////////////////////
void DanteBrowser::OnSocketsChangedCallback(const db_browse_t * browse)
{
    Q_UNUSED(browse)
    //在run()中轮询更新socket，效率稍差，代码简单
//    DanteBrowser *pBrowser = (DanteBrowser *)db_browse_get_context(browse);
//    pBrowser->updateSockets();
}

void DanteBrowser::OnNetworkChangedCallback(const db_browse_t * browse)
{
    Q_UNUSED(browse)
//    DanteBrowser * pBrowser = (DanteBrowser *)db_browse_get_context(browse);
}

void DanteBrowser::OnNodeChangedCallback(db_browse_t *browse, const db_node_t *node, db_node_change_t node_change)
{
     DanteBrowser * pBrowser = (DanteBrowser *)db_browse_get_context(browse);
     pBrowser->handleNodeChanged(node, node_change);
}

void DanteBrowser::OnDeviceChangedCallback(dr_device_t * device, dr_device_change_flags_t change_flags)
{
    DanteBrowser *pBrowser = (DanteBrowser *)dr_device_get_context(device);
    if (change_flags & DR_DEVICE_CHANGE_FLAG_ADDRESSES)
    {        
        dante_ipv4_address_t address;
        aud_error_t result = dr_device_get_address(device, &address);
        if (result != AUD_SUCCESS)
        {
            if(pBrowser!=NULL) pBrowser->printAudInfo("DanteBrowser::OnDevicesChangedCallback()->dr_device_get_address()", result);
        }
        const char* name = dr_device_get_name(device);

        QString ip = QHostAddress(ntohl(address.host)).toString();
        if(pBrowser!=NULL) pBrowser->tempDeviceUpdate(name, ip);
    }
}

DanteBrowser::DanteBrowser(QObject *parent) :QThread(parent)
{
//    _primaryNetIndex = 0;
//    _slaverNetIndex	= 0;
    //设置select()函数的超时时间为默认的1秒钟
    _selectTimout.tv_sec	= 0;
    _selectTimout.tv_usec = 200*1000;

    resetParams();//重启browser需要设置的参数
}


DanteBrowser::~DanteBrowser(void)
{
    close();
    //删除TempDevice的QMap
    QMapIterator<QString, TempDevice*> iter(_tempDeviceMap);
    while (iter.hasNext())
    {
        delete iter.value();
    }

}

//////////////////////////////////////////////////////////////
/// 主线程函数
void DanteBrowser::run()
{
#ifdef PRINT_AUDINATE_DEVICE_SEARCH_DEBUG
    qDebug()<<"Enter DanteBrowser::run()";
#endif

    aud_utime_t nextActionTime;
    memset(&nextActionTime, 0, sizeof(aud_utime_t));

    while (true)
    {
        if(cacheSockets()== false)
        {
            return;
        }
        aud_error_t result = AUD_SUCCESS;


        // 查询并处理browser事件
        int	select_result = select(_cached_sockets.read_fds.fd_count,
                        &_cached_sockets.read_fds, NULL, NULL, &_selectTimout);	//等待socket读数据事件
        if (select_result == SOCKET_ERROR)
        {
            result = aud_error_get_last();
            printAudInfo("DanteBrowser::run() ->select(), error:", result);
            return;
        }
        else if(select_result >0)
        {
            //处理socket事件，触发回调函数，_cached_sockets必需是select()之后的socket
            result = db_browse_process(_browser, &_cached_sockets, &nextActionTime);
            if (result != AUD_SUCCESS)
            {
                printAudInfo("DanteBrowser::run()->db_browse_process()", result);
                return;
            }
         }
        else if (nextActionTime.tv_sec || nextActionTime.tv_usec)
        {
            aud_utime_t now;
            aud_utime_get(&now);
            if (aud_utime_compare(&nextActionTime, &now) < 0)
            {
                result = db_browse_process(_browser, &_cached_sockets, &nextActionTime);
                if (result != AUD_SUCCESS)
                {
                    printAudInfo("DanteBrowser::run()->db_browse_process()", result);
                    return;
                }
            }
        }

        // 查询并处理route事件
        dante_sockets_t routeSockets;
        dr_devices_get_sockets(_localDevices, &routeSockets);
        if (select(routeSockets.n, &routeSockets.read_fds, NULL, NULL, &_selectTimout) > 0)
        {
            result = dr_devices_process(_localDevices, &routeSockets);
            if (result != AUD_SUCCESS)
            {
                printAudInfo("DanteBrowser::run()->db_browse_process()", result);
                return;
            }
        }

        aud_utime_t curTime;
        result = aud_utime_get(&curTime);
        //确认设备是否在线,应对网线被拔等情景
        if (aud_utime_compare(&curTime, &_nextConfirmTime )>0)
        {
            confirmDevice();
        }

        if (aud_utime_compare(&curTime, &_nextRediscoverTime )>0)
        {
            result = db_browse_rediscover(_browser, _browseTypes);
            if (result != AUD_SUCCESS)
            {
                printAudInfo("DanteBrowser::run()->db_browse_rediscover()", result);
                return;
            }
            _nextRediscoverTime.tv_sec += REDISCOVER_DEVICE_PERIOD;
        }
    }
#ifdef PRINT_AUDINATE_DEVICE_SEARCH_DEBUG
    qDebug()<<"Exit DanteBrowser::run()";
#endif
}

void DanteBrowser::handleNodeChanged(const db_node_t *node, db_node_change_t changeType)
{
    if(node->type != DB_NODE_TYPE_DEVICE) return;
    const db_browse_device_t * device = node->_.device;
    if(changeType == DB_NODE_CHANGE_REMOVED)
    {
        const char * name = db_browse_device_get_name(device);
        tempDeviceRemoved(name);
    }
    else
    {
        const char * deviceDesp = db_browse_device_get_router_info(device); //设备类型描述
        const char * name = db_browse_device_get_name(device);				//设备名称
        const conmon_instance_id_t *pID = db_browse_device_get_instance_id(device);

        if(*deviceDesp == '\0' || pID == NULL) return;
        //获取MAC地址
        dante_device_id_str_t buff;
        dante_device_id_to_string(&pID->device_id, buff );
        QString mac = QString(buff).toUpper();
        if (mac.right(4) ==("0000")) mac.remove("0000");
        else if (mac.contains("FFFE"))  mac.remove("FFFE");
        tempDeviceAdd(name, mac, deviceDesp);

        //异步获取ip地址
        dr_device_t * device = NULL;
        aud_error_t result = dr_device_open_remote(_localDevices, name, &device);
        if (result != AUD_SUCCESS)
        {
            printAudInfo("DanteBrowser::processSocketEvent()->dr_device_open_remote()", result);
        }

        dr_device_set_context(device, this);
        dr_device_set_changed_callback(device, &OnDeviceChangedCallback);
    }
}

void DanteBrowser::removeDevice(QString name)
{
    if(_tempDeviceMap.contains(name))//如果包含 name
    {
        delete _tempDeviceMap[name];
        _tempDeviceMap.remove(name);
    }
}

void DanteBrowser::tempDeviceRemoved(QString name)
{
    if(_tempDeviceMap.contains(name))
    {
        QString mac = _tempDeviceMap[name]->_mac;
        delete _tempDeviceMap[name];
        _tempDeviceMap.remove(name);
        emit deviceRemoved(mac);
#ifdef PRINT_AUDINATE_DEVICE_SEARCH_DEBUG
        qDebug()<<"  tempDeviceRemoved("<<mac <<")";
#endif
    }
}

void DanteBrowser::tempDeviceAdd(QString name, QString mac, QString deviceDesp)
{
    TempDevice* tempDevice = NULL;
    if(_tempDeviceMap.contains(name))
    {
        tempDevice = _tempDeviceMap[name];
    }
    else
    {
       tempDevice = new TempDevice;       
       _tempDeviceMap.insert(name, tempDevice);
    }
    tempDevice->_name = name;
    tempDevice->_mac = mac;
    tempDevice->_deviceDesp = deviceDesp;
}

void DanteBrowser::tempDeviceUpdate(QString name, QString ip)
{
    if(_tempDeviceMap.contains(name))
    {
        TempDevice* tempDevice = _tempDeviceMap[name];
        if(ip != tempDevice->_ip)
        {
            tempDevice->_ip =ip;
            emit deviceAdded(tempDevice->_mac, ip, name, tempDevice->_deviceDesp);
#ifdef PRINT_AUDINATE_DEVICE_SEARCH_DEBUG
            qDebug()<<"  deviceAdded("<<name <<"," <<tempDevice->_mac<< ","
                <<ip <<"," <<tempDevice->_deviceDesp<< ")";
#endif
        }
    }
}

bool DanteBrowser::resetParams(void)
{
    _danteEnv = NULL;
    _localDevices = NULL;
    _browser = NULL;
    _browseTypes = DB_BROWSE_TYPE_MEDIA_DEVICE | DB_BROWSE_TYPE_CONMON_DEVICE;

    dante_sockets_clear(&_cached_sockets);
    return true;
}


void DanteBrowser::open()
{
    resetParams();

    aud_error_t result = AUD_SUCCESS;

    db_browse_config_t	browse_config;				//定义设备浏览器所使用的网络接口
    db_browse_config_init_defaults(&browse_config);	//初始化默认的浏览器配置方案

    _browseTypes = DB_BROWSE_TYPE_MEDIA_DEVICE | DB_BROWSE_TYPE_CONMON_DEVICE;	//仅浏览Dante音频设备和ConMon设备

    //双网卡设置
//    _primaryNetIndex	= primaryNetIndex;
//    _slaverNetIndex = slaverNetIndex;
//    browse_config.interface_indexes[0] = primaryNetIndex;		// OsInterfaceIndex = 0 默认是第一个可用的网络接口
//    browse_config.interface_indexes[1] = slaverNetIndex;		// OsInterfaceIndex = 0 默认是第一个可用的网络接口
//    browse_config.num_interface_indexes = 2;	//使用两个网卡接口来发现设备
//    browse_config.localhost = AUD_TRUE;
    browse_config.interface_indexes[0] = Config::instance()->getCurIpIndex();
    //创建Dante env
    result = aud_env_setup(&_danteEnv);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("DanteBrowser::setupBrowser()->aud_env_setup():", result);
        return;
    }
    //创建Dante browser
    result = db_browse_new(_danteEnv, _browseTypes, &_browser);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("DanteBrowser::setupBrowser()->db_browse_new():" , result);
        return;
    }

    //创建本地device
    result = dr_devices_new(_danteEnv, &_localDevices);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("DanteBrowser::setupBrowser()->dr_devices_new():" , result);
        return;
    }

    dr_devices_set_num_handles(_localDevices, MAX_DEVICE_HANDLERS);
    dr_devices_set_request_limit(_localDevices, MAX_DEVICE_HANDLERS);

    result = db_browse_set_max_sockets(_browser, MAX_SOCKETS);
    if (result != AUD_SUCCESS)
    {
        printAudInfo("DanteBrowser::setupBrowser()->db_browse_set_max_sockets():", result);
        return;
    }

    //设置回调函数
    db_browse_set_sockets_changed_callback(_browser, OnSocketsChangedCallback);
    db_browse_set_network_changed_callback(_browser, OnNetworkChangedCallback);
    db_browse_set_node_changed_callback(_browser, OnNodeChangedCallback);

    db_browse_set_context(_browser, this);

    result = db_browse_start_config(_browser, &browse_config);	//启动设备浏览器
    if (result != AUD_SUCCESS)
    {
        printAudInfo("DanteBrowser::setupBrowser()->db_browse_start_config():", result);
        return;
    }

    aud_utime_t curTime;
    aud_utime_get(&curTime);
    _nextConfirmTime = curTime;
    _nextRediscoverTime = curTime;
    _nestRestartTime = curTime;

//    printAudInfo("DanteBrowser::Open() seccess");

    start();
}

void DanteBrowser::close()
{
    terminate();//中转机线程的执行,后面使用Qthread::wait()来确保
    wait();

    if(_localDevices)
    {
        dr_devices_delete(_localDevices);
        _localDevices = NULL;
    }
    if (_browser)
    {
        db_browse_delete(_browser);
        _browser = NULL;
    }
    if(_danteEnv){
        aud_env_release(_danteEnv);
        _danteEnv = NULL;
    }
}

void DanteBrowser::resetBrowser(void)
{
    close();
    open();
}


void DanteBrowser::printAudInfo( const char* errorInfo, aud_error_t result)
{
#ifdef PRINT_AUDINATE_DEVICE_SEARCH_DEBUG
    if(result != AUD_SUCCESS){
        aud_errbuf_t errbuf;
        aud_error_message(result, errbuf);
        qDebug()<<errorInfo << errbuf;
    }
    else qDebug()<<errorInfo;
#endif
}



void DanteBrowser::confirmDevice(void)
{
    aud_error_t result;
    unsigned int i;
    const db_browse_network_t * network = db_browse_get_network(_browser);
    for (i = 0; i < db_browse_network_get_num_devices(network); i++)
    {
        db_browse_device_t * device = db_browse_network_device_at_index(network, i);
        result = db_browse_device_reconfirm(device, 0, AUD_FALSE);
        if (result != AUD_SUCCESS)
        {   //该错误可忽略
            printAudInfo("DanteBrowser::confirmDevice(),Error:", result);
            return;
        }
    }

    _nextConfirmTime.tv_sec += CONFIRM_DEVICE_PERIOD;
}



bool DanteBrowser::cacheSockets(void)
{
    dante_sockets_clear(&_cached_sockets);						//首先清空Socket缓冲区
    aud_error_t result = db_browse_get_sockets(_browser, &_cached_sockets);	//备份当前正在使用的socket
    if (result != AUD_SUCCESS)
    {
        printAudInfo("DanteBrowser::updateSockets()->db_browse_get_sockets():", result);
        return false;
    }
    return true;
}



QString DanteBrowser::getChangeTypeDesc(db_node_change_t changeType)
{
    switch(changeType)
    {
    case DB_NODE_CHANGE_ADDED:
        return "#A#";
    case DB_NODE_CHANGE_MODIFIED:
        return "#M#";
    case DB_NODE_CHANGE_REMOVED:
        return "#D#";
    }
    return "#?#";
}




