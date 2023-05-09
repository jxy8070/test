#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T09:34:29
#
#-------------------------------------------------
PRECOMPILED_HEADER = stable.h
QT       += core gui multimedia network widgets xml sql websockets webenginewidgets
#CONFIG += qt windows thread
#增加“管理员权限运行”功能
QMAKE_LFLAGS+= /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO     #release版本可调试
QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG                       #release版也将生成“.pdb”后缀的调试信息文件
LIBS += -lDbgHelp                                                   #调用库

TEMPLATE = app

TRANSLATIONS += language/SimpliedChinese.ts  language/English.ts
#TRANSLATIONS += language/FangTu/FangTu_SimpliedChinese.ts  language/FangTu/FangTu_English.ts

DEFINES += OUTPUT_EQ_10SEGMENT QT_DEBUGX Qt_NO_CAST_FROM_ASCII

INCLUDEPATH += $$PWD/Lua/LuaSrc 
DEPENDPATH += $$PWD/Lua/LuaSrc


#配置不同的客户版本
#福川:Fitcan; 方图:FangTu; 东方佳联:KEMU; 东方佳联:ControlMax; 广州声拓:GuangZhouShengTuo;
#湖山:HuShan; 北京慧声:BeiJingHuiSheng; 谷睿电子:GuRuiDianZi; 易创:YiChuang;
#百联达:BaiLianDa; 富通亚讯:FTYX;
CONFIG += KEMU

Fitcan{ #福川
    DEFINES += Logo_SuZhouFitcan
    TARGET = Designer
    RC_ICONS += ico/Fitcan.ico
}
FangTu {#方图
    DEFINES += Logo_FangTu
    TARGET = "FT-Designer"
    RC_ICONS += ico/FangTu.ico
}
KEMU {#东方佳联KEMU
    DEFINES += Logo_DongFangJiaLian
    TARGET = "KEMU-Designer"
    RC_ICONS += ico/KEMU.ico
}
ControlMax {#东方佳联ControlMax
    DEFINES += Logo_ControlMax
    TARGET = "ControlMax Designer"
    RC_ICONS += ico/ControlMax.ico
}
GuangZhouShengTuo {#广州声拓
    DEFINES += Logo_GuangZhouShengTuo
    TARGET = "EAJAX AudioDSP"
    RC_ICONS += ico/GuangZhouShengTuo.ico
}
HuShan {#湖山
    DEFINES += Logo_HuShan
    TARGET = "HS_Designer"
    RC_ICONS += ico/HuShan.ico
}
BeiJingHuiSheng { #北京慧声(三所)
    DEFINES += Logo_BeiJingHuiSheng
    TARGET = Ceiadap
    RC_ICONS += ico/Ceiadap.ico
}
GuRuiDianZi{ #谷睿电子
    DEFINES += Logo_GuRuiDianZi
    TARGET = AoIP
    RC_ICONS += ico/GuRuiDianZi.ico
}
YiChuang{ #易创
    DEFINES += Logo_YiChuang
    TARGET = EAV-martix
    RC_ICONS += ico/Yichuang.ico
}
BaiLianDa {#百联达
    DEFINES  += Logo_BaiLianDa
    TARGET    = "BLD-Designer"
    RC_ICONS += ico/BaiLianDa.ico #【2022-11-17】百联达：替换logo；设备列表只保留 NAP-200一个设备型号，其它与Fitcan相同。
}
FTYX{ #富通亚讯
    DEFINES += Logo_FTYX
    TARGET = Desiner
    RC_ICONS += ico/Fitcan.ico
}


LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 \
    -loleaut32 -luuid -lodbc32 -lodbccp32 -lws2_32 -liphlpapi -lcrypt32

CONFIG(debug, debug|release) {
    LIBS += -LAES70Lib/Win64/Debug -lAES70DeviceManager -lAES70Helper
    OBJECTS_DIR = temp/debugObj
} else {
    LIBS += -LAES70Lib/Win64/Release -lAES70DeviceManager -lAES70Helper
    OBJECTS_DIR = temp/releaseObj
}

MOC_DIR = temp/moc
RCC_DIR = temp/rcc
UI_DIR = temp/ui

include(Lua/Lua.pri)
include(LuaIde/LuaIde.pri)
include(Frame/Frame.pri)
include(UserManager/UserManager.pri)
include(Protocal/Protocal.pri)
include(PropertyBrowser/PropertyBrowser.pri)
include(deviceSetting/_DeviceSetting.pri)
include(configSetting/_ConfigSetting.pri)
include(customWidget/_CustomWidget.pri)
include(CtrlItem/CtrlItem.pri)

#开启dante支持，需在DEFINES中添加DanteSupported,并取消下一语句注释
#include(DanteSdk/DanteSdk.pri)
if(contains(DEFINES,DanteSupported)){
    CONFIG(debug, debug|release) {
        LIBS += -LdanteSdk -ldapi_64d -ldante_dnssd_64d -ljansson_64d -llibcurl_64d -lzlibstatic_64d -llibeay32MD_64d -lssleay32MD_64d
    } else {
        LIBS += -LdanteSdk -ldapi_64 -ldante_dnssd_64 -ljansson_64 -llibcurl_64 -lzlibstatic_64 -llibeay32MD_64 -lssleay32MD_64
    }
}


SOURCES += main.cpp\
    UserControlInterface/devicestartuswindow.cpp \
    UserControlInterface/uciitem_mixer.cpp \
    UserControlInterface/uciitem_port.cpp \
    UserControlInterface/uciitemview.cpp \
    UserControlInterface/usercontrolinterface.cpp \
    configItem/configitemaudioplayer.cpp \
    configItem/configitemaudiorecord.cpp \
    configItem/configitemcrossfader.cpp \
    configItem/configitemgain.cpp \
    configItem/configitemgainramp.cpp \
    configItem/configitempriorityducker.cpp \
    configItem/configitemsoftaec.cpp \
    configItem/configitemuci.cpp \
    device/device_amp_dn8000.cpp \
    deviceItem/deviceitem_amp_dn8000.cpp \
    linelabel.cpp \
    luapinparam_pc.cpp \
    skinstyle.cpp \
    mfrscfg.cpp \
    monitor/resourcemanager.cpp \
    serpentineconnector.cpp \
    snapshotSaveData/snapshotsaveaecdata.cpp \
    snapshotSaveData/snapshotsaveautomixdata.cpp \
    snapshotSaveData/snapshotsavecrossfaderdata.cpp \
    snapshotSaveData/snapshotsavedelaydata.cpp \
    snapshotSaveData/snapshotsavedyndata.cpp \
    snapshotSaveData/snapshotsaveeqdata.cpp \
    snapshotSaveData/snapshotsavefiltedata.cpp \
    snapshotSaveData/snapshotsavefreqshiftdata.cpp \
    snapshotSaveData/snapshotsavegaindata.cpp \
    snapshotSaveData/snapshotsavegainrampdata.cpp \
    snapshotSaveData/snapshotsavegpidata.cpp \
    snapshotSaveData/snapshotsaveinputdata.cpp \
    snapshotSaveData/snapshotsavemeterdata.cpp \
    snapshotSaveData/snapshotsavemixerdata.cpp \
    snapshotSaveData/snapshotsavenfcdata.cpp \
    snapshotSaveData/snapshotsaveobject.cpp \
    snapshotSaveData/snapshotsaveoutputdata.cpp \
    snapshotSaveData/snapshotsavepriorityduckerdata.cpp \
    snapshotSaveData/snapshotsaverouterdata.cpp \
    snapshotSaveData/snapshotsavegainrampdata.cpp \
    snapshotSaveData/snapshotsavesignaldata.cpp \
    snapshotSaveData/snapshotsavesplitdata.cpp \
    stable.cpp \
    ItemPort.cpp \
    LineItem.cpp \
    Designitem.cpp \
    utils.cpp \
    config.cpp \
    device/devicemanager.cpp \
    device/devicemodel.cpp \
    deviceItem/concretedeviceitem.cpp \
    deviceItem/deviceItem.cpp \
    deviceItem/deviceitem_spk.cpp \
    deviceItem/deviceitemfactory.cpp \
    device/device_spk.cpp \
    configItem/configitem.cpp \
    deviceItem/deviceitem_dvs.cpp \
    configItem/configitemfactory.cpp \
    configItem/configiteminput.cpp \
    configItem/configitemoutput.cpp \
    configItem/configitemdelay.cpp \
    configItem/configitemrouter.cpp \
    configItem/configitemdyn.cpp \
    configItem/configitemeq.cpp \
    configItem/configitemfilte.cpp \
    configItem/configitemsplit.cpp \
    configItem/configitemsignal.cpp \
    configItem/configitemmeter.cpp \
    configItem/configitemmixer.cpp \
    monitor/monitorwindows.cpp \
    deviceItem/deviceitem_trn.cpp \
    device/device_trn.cpp \
    monitor/monitor_spk.cpp \
    monitor/monitor_amp.cpp \
    configItem/configitemgpi.cpp \
    device/device_amp_quad.cpp \
    deviceItem/deviceitem_amp_quad.cpp \
    device/device_monitor_spk.cpp \
    deviceItem/deviceitem_monitor_spk.cpp \
    configItem/configitemautomaticmixer.cpp \
    configItem/configitemfreqshift.cpp \
    device/device_mic_quad.cpp \
    deviceItem/deviceitem_mic_quad.cpp \
    sysdbutility.cpp \
    device/AES70device.cpp \
    configItem/configitemaec.cpp \
    device/device_amp_0_2_line.cpp \
    deviceItem/deviceitem_amp_0_2_line.cpp \
    device/device_amp_2_0_mic_line.cpp \
    deviceItem/deviceitem_amp_2_0_mic_line.cpp \
    device/device_stm32_poe_0_2.cpp \
    deviceItem/deviceitem_stm32_poe_0_2.cpp \
    device/device_stm32_poe_0_1.cpp \
    deviceItem/deviceitem_stm32_poe_0_1.cpp \
    deviceItem/deviceitem_stm32_poe_1_0.cpp \
    device/device_stm32_poe_1_0.cpp \
    device/device_rt1052_poe_60w_2_4.cpp \
    deviceItem/deviceitem_rt1052_poe_60w_2_4.cpp \
    configItem/configitemmicrophonecontrol.cpp \
    deviceItem/deviceitem_rt1052_amp_module_4_4.cpp \
    deviceItem/deviceitem_room.cpp \
    device/device_amp_module.cpp \
    deviceItem/deviceitem_monitoring_aids.cpp \
    device/device_monitoring_aids.cpp \
    monitoring_aids/monitorproperty.cpp \
    monitoring_aids/monitoringfactory.cpp \
    monitoring_aids/monitorblackstage.cpp \
    monitoring_aids/concretemonitoritem.cpp \
    monitoring_aids/monitoritem.cpp \
    monitoring_aids/monitoitemfactory.cpp \
    monitoring_aids/datareceive.cpp \
    device/device_rt1052_AP400.cpp \
    deviceItem/deviceitem_rt1052_AP400.cpp \
    device/device_rt1052_poe_AP60_Bluetooth.cpp \
    deviceItem/deviceitem_rt1052_poe_AP60_Bluetooth.cpp \
    configItem/configitemFIRCustom.cpp \
    configItem/configitemsnapshot.cpp \
    configItem/configitemNFC.cpp \ 
    utils_TimeSpace.cpp

HEADERS  += stable.h \
    ItemPort.h \
    LineItem.h \
    DesignItem.h \
    UserControlInterface/devicestartuswindow.h \
    UserControlInterface/uciitem_mixer.h \
    UserControlInterface/uciitem_port.h \
    UserControlInterface/uciitemview.h \
    UserControlInterface/usercontrolinterface.h \
    configItem/configitemaudioplayer.h \
    configItem/configitemaudiorecord.h \
    configItem/configitemcrossfader.h \
    configItem/configitemgain.h \
    configItem/configitemgainramp.h \
    configItem/configitempriorityducker.h \
    configItem/configitemsoftaec.h \
    configItem/configitemuci.h \
    device/device_amp_dn8000.h \
    deviceItem/deviceitem_amp_dn8000.h \
    linelabel.h \
    luapinparam_pc.h \
    skinstyle.h \
    mfrscfg.h \
    monitor/resourcemanager.h \
    serpentineconnector.h \
    snapshotSaveData/snapshotsaveaecdata.h \
    snapshotSaveData/snapshotsaveautomixdata.h \
    snapshotSaveData/snapshotsavecrossfaderdata.h \
    snapshotSaveData/snapshotsavedelaydata.h \
    snapshotSaveData/snapshotsavedyndata.h \
    snapshotSaveData/snapshotsaveeqdata.h \
    snapshotSaveData/snapshotsavefiltedata.h \
    snapshotSaveData/snapshotsavefreqshiftdata.h \
    snapshotSaveData/snapshotsavegaindata.h \
    snapshotSaveData/snapshotsavegainrampdata.h \
    snapshotSaveData/snapshotsavegpidata.h \
    snapshotSaveData/snapshotsaveinputdata.h \
    snapshotSaveData/snapshotsavemeterdata.h \
    snapshotSaveData/snapshotsavemixerdata.h \
    snapshotSaveData/snapshotsavenfcdata.h \
    snapshotSaveData/snapshotsaveobject.h \
    snapshotSaveData/snapshotsaveoutputdata.h \
    snapshotSaveData/snapshotsavepriorityduckerdata.h \
    snapshotSaveData/snapshotsaverouterdata.h \
    snapshotSaveData/snapshotsavegainrampdata.h \
    snapshotSaveData/snapshotsavesignaldata.h \
    snapshotSaveData/snapshotsavesplitdata.h \
    utils.h \
    common.h \
    config.h \
    device/devicemanager.h \
    device/devicemodel.h \
    deviceItem/concretedeviceitem.h \
    deviceItem/deviceItem.h \
    deviceItem/deviceitem_spk.h \
    deviceItem/deviceitemfactory.h \
    device/device_spk.h \
    configItem/configitem.h \
    deviceItem/deviceitem_dvs.h \
    configItem/configitemfactory.h \
    configItem/configiteminput.h \
    configItem/configitemoutput.h \
    configItem/configitemdelay.h \
    configItem/configitemrouter.h \
    configItem/configitemdyn.h \
    configItem/configitemeq.h \
    configItem/configitemfilte.h \
    configItem/configitemsplit.h \
    configItem/configitemsignal.h \
    configItem/configitemmeter.h \
    configItem/configitemmixer.h \
    monitor/monitorwindows.h \
    deviceItem/deviceitem_trn.h \
    device/device_trn.h \
    monitor/monitor_amp.h \
    monitor/monitor_spk.h \
    configItem/configitemgpi.h \
    device/device_amp_quad.h \
    deviceItem/deviceitem_amp_quad.h \
    device/device_monitor_spk.h \
    deviceItem/deviceitem_monitor_spk.h \
    configItem/configitemautomaticmixer.h \
    configItem/configitemfreqshift.h \
    device/device_mic_quad.h \
    deviceItem/deviceitem_mic_quad.h \
    sysdbutility.h \
    device/AES70device.h \
    configItem/configitemaec.h \
    device/device_amp_0_2_line.h \
    deviceItem/deviceitem_amp_0_2_line.h \
    device/device_amp_2_0_mic_line.h \
    deviceItem/deviceitem_amp_2_0_mic_line.h \
    device/device_stm32_poe_0_1.h \
    device/device_stm32_poe_0_2.h \
    device/device_stm32_poe_1_0.h \
    deviceItem/deviceitem_stm32_poe_0_2.h \
    deviceItem/deviceitem_stm32_poe_0_1.h \
    deviceItem/deviceitem_stm32_poe_1_0.h \
    device/device_rt1052_poe_60w_2_4.h \
    deviceItem/deviceitem_rt1052_poe_60w_2_4.h \
    configItem/configitemmicrophonecontrol.h \
    deviceItem/deviceitem_rt1052_amp_module_4_4.h \
    deviceItem/deviceitem_room.h \
    device/device_amp_module.h \
    deviceItem/deviceitem_monitoring_aids.h \
    device/device_monitoring_aids.h \
    monitoring_aids/monitorproperty.h \
    monitoring_aids/monitoringfactory.h \
    monitoring_aids/monitorblackstage.h \
    monitoring_aids/concretemonitoritem.h \
    monitoring_aids/monitoritem.h \
    monitoring_aids/monitoitemfactory.h \
    monitoring_aids/datareceive.h \
    device/device_rt1052_AP400.h \
    deviceItem/deviceitem_rt1052_AP400.h \
    AES70Lib/SDK_INCLUDE/AES70Connectors.h \
    AES70Lib/SDK_INCLUDE/AES70Device.h \
    AES70Lib/SDK_INCLUDE/AES70DeviceRoutingData_Added_by_HLX.h \
    AES70Lib/SDK_INCLUDE/AES70DeviceSystemCfg.h \
    AES70Lib/SDK_INCLUDE/AES70ManagerDataType.h \
    AES70Lib/SDK_INCLUDE/AES70ManagerExport.h \
    AES70Lib/SDK_INCLUDE/AES70StreamConnectionParam.h \
    AES70Lib/SDK_INCLUDE/AppNetIfCfg.h \
    AES70Lib/SDK_INCLUDE/DeclareExportAES70ExtDLL.h \
    device/device_rt1052_poe_AP60_Bluetooth.h \
    deviceItem/deviceitem_rt1052_poe_AP60_Bluetooth.h \
    configItem/configitemFIRCustom.h \
    configItem/configitemsnapshot.h \
    configItem/configitemNFC.h \

FORMS    += \
    monitor/monitor_amp.ui \
    monitor/monitor_spk.ui \
    monitor/monitorwindows.ui \
    monitoring_aids/monitorproperty.ui \

RESOURCES += Image.qrc \
    style/qss.qrc

DISTFILES += \
    language/SystemTranslation.ts \
    readme.txt

