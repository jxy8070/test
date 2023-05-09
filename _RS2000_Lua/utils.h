#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include <QObject>
#include <QMessageBox>
#include <QDebug>

class LineLabel;
class ConfigItemSnapshot;
class QSettings;
class Utils:public QObject
{
    Q_OBJECT
public:
    typedef struct devItemPorts {
        struct In{
            uint s_ANum;    //Number of Input Analog port
            uint s_NNum;    //Number of Input Network port
            uint s_DNum;    //Number of Input Dante Port
            uint s_UNum;    //Number of Input USB port
        }s_In;
        struct Out{
            uint s_ANum;    //Number of Output Analog port
            uint s_NNum;    //Number of Output Network port
            uint s_DNum;    //Number of Output Dante Port
            uint s_UNum;    //Number of Output USB port
        }s_Out;
        devItemPorts(uint8_t isUsing, const In input, const Out output) {
            memset(this, 0, sizeof (struct devItemPorts));

            if(isUsing & (0x01 << 7)) s_In.s_ANum = input.s_ANum;
            if(isUsing & (0x01 << 6)) s_In.s_NNum = input.s_NNum;
            if(isUsing & (0x01 << 5)) s_In.s_DNum = input.s_DNum;
            if(isUsing & (0x01 << 4)) s_In.s_UNum = input.s_UNum;
            if(isUsing & (0x01 << 3)) s_Out.s_ANum = output.s_ANum;
            if(isUsing & (0x01 << 2)) s_Out.s_NNum = output.s_NNum;
            if(isUsing & (0x01 << 1)) s_Out.s_DNum = output.s_DNum;
            if(isUsing & (0x01 << 0)) s_Out.s_UNum = output.s_UNum;
        }
        devItemPorts() {
            memset(this, 0, sizeof (struct devItemPorts));
        }
    }devItemPorts_t;

    typedef struct FirmwareUpdate {//dsp升级
        uint s_FW[4];       //固件版本号
        uint s_HW;          //硬件版本号
        uint s_PFType;      //处理器类型
        char *s_fileName;   //升级文件名
    }FirmwareUpdate_t;

    typedef struct SoftwareVersion {//软件版本
        char s_verPrefix   = 'V';   //版本号前缀     方图: 'V'正式版，'T'测试版
        int s_transCtrlMajor = 2;   //大版本更新使用
        int s_transCtrlMinor = 0;   //协议变动使用
        int s_transCtrlBuild = 1;   //新增模块使用
        int s_pcBuild       = 16;
        QString structToString(void) {
            return QString("%1%2.%3.%4.%5").arg(s_verPrefix)
                                           .arg(s_transCtrlMajor)
                                           .arg(s_transCtrlMinor)
                                           .arg(s_transCtrlBuild)
                                           .arg(s_pcBuild);
        }
    }SoftwareVersion_t;

public:
    static void MessageBoxInfo(const QString &title, const QString &text, QPixmap pixmap = QPixmap());
    static void MessageBoxWarning(const QString &title, const QString &text);
    static QMessageBox::StandardButton MessageBoxQuestion(const QString &title, const QString &text,
                               QMessageBox::StandardButtons buttons = QMessageBox::NoButton);
    static PlatformType getPlatformType(DevType devType);
    static QString getCfgTypeName(CfgType type, bool isTr = false);
    static QString getCfgPortModelName(bool isInput, DevPortType devType, int logicCh, int portGroupNum);
    static QString getCfgPortChannelName(DevPortType devType, int logicCh, QString subfix = QString());
    static QString getCfgModelName_FangTu(CfgType type);
    static QString getCfgModelName_Other(CfgType type);
    static QString getCfgGroupName(CfgType type);
    static QString getCfgShortName(CfgType type);
    static float getSpaceUsage(DevType devType, CfgType cfgType);
    static float getTimeUsage(DevType devType, CfgType cfgType);
    static devItemPorts_t getDeviceItemPortNum(DevType devType, uint8_t isUsing = 0xFF);
    static int getDevTatalPortNum(DevType devType);
    static bool RegCheck_IsIp(QString ipStr);
    static QString getNewName(QString oldName = QString());

    static void saveRoomInputOutputForSolutionFile(QString roomName, QMap<QString, LineLabel *> inLineLab,
                                                   QMap<QString, LineLabel *> outLineLab);

    static QList<QString> insertionSort(QList<QString>& list);

    static void setMaxMasterVersion(CfgType type);
    //Historical filePath
    static void saveProjectExplorer(QString RecentProject);
    static void loadProjectExplorer(QStringList &RecentProjects);
    static void removeProjectExplorer(QString RecentProject);
    //Default working path
    static void saveDefaultWorkingPath(QString defPath);
    static void loadDefaultWorkingPath(QString &defPath);
    //Save current open pages
    static void saveCurOpenPagesData(const QStringList& pageNames, const QString& activeWinName);
    static void loadCurOpenPagesData(QStringList& pageNames, QString& activeWinName);
    //Get Software Version Number
    static QString getSoftwareVersion(void);

public:
    static bool ShowMsgBoxFrameless;
    static quint8 maxMasterVersion[4];  //0~3:主控版本号，4: 主控相关银硬件号
};

#endif // UTILS_H
