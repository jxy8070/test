#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>
#include <QtSql>
#include <QNetworkInterface>
#include "common.h"
#include "mfrscfg.h"
#include "skinstyle.h"

#define DEVICE_SETTING_FILENAME     "conf/deviceSetting.conf"

typedef struct {
    int channelType;
    int channelValid;
}ChannelInfoStruct;


//用户信息
struct UserInfo {
    QString type;       //用户类型
    QString name;       //用户名
    QString password;   //用户密码
};

enum LanguageEnum {
    LANGUAGE_SIMPLIED_CHINESE = 0,
    LANGUAGE_ENGLISH,
};

class QDomDocument;
class QDomNode;
class SysDBUtility;
class Config : public QObject
{
    Q_OBJECT

public: //function
    void setLanguage(LanguageEnum language);
    void changUserInfo(int i, UserInfo usrT);
    void addNewUser();
    void deleteUser(int i);
    bool hasAdminPriv();
    bool hasNormalPriv();

    //删除失效、废弃的翻译(启用手动添加的系统翻译)
    bool remvoeTsFileVanishedAndObsolete(const QString &srcFileName, const QString &destFileName);
    void setCurNetInfo(QString curIp, QNetworkInterface curNet);
    QString getSoftwareVersion(void);
public:
    void init();
    inline UserInfo& getCurUser() {return _curUser;}
    inline QList<UserInfo>& getUserList() {return _userList;}
    inline QString getCurIp(){return _curIp;}
    inline int getCurIpIndex() {return _curNet.index(); }
    inline QNetworkInterface getCurItr() {return _curNet;}
    inline SysDBUtility *getAlarmDBBase(void) {return _dbUtility;}
    inline LanguageEnum getLanguage(void) {return _curLang;}
    inline int getPortGroupNum(void) {return _portGroupNum;}
    static inline bool isLuaSupported() {return false;}

private:
    QVariant getInitValue(QString node, QString key);
    void setInitValue(QString node, QString key, QVariant value);
    void initManufacturerCfg();


private://private member
    QNetworkInterface _curNet;  //提供主机的IP地址和网络接口的列表
    QString _curIp;
    UserInfo _curUser;
    QList<UserInfo> _userList;
    SysDBUtility *_dbUtility;
    QSettings *_initSetting;
    LanguageEnum _curLang;
    int _portGroupNum;
    MfrsCfg _curMfrsCfg;        //当前客户配置
    SkinStyle *_curSkin;        //当前皮肤样式

///////////////////////////////////////////////////////
/// /// 实现Singleton模式相关代码
public:
    static Config *instance();
    static QByteArray SystemTranslation();
    static MfrsCfg& curMfrsCfg();
    static SkinType curSkinType();
    static SkinStyle *curSkinStyle();
    static void ChangeSkin(SkinType newSkin);
    static bool ShowProgressDialog;
    static bool ShowSaveDialog;
private:
    Config(QObject *parent = 0);        //禁止构造函数。
    Config(const Config&);              //禁止拷贝构造函数。
    Config & operator=(const Config&);  //禁止赋值拷贝函数。
    static QMutex Mutex;                //实例互斥锁。
    static Config* _Instance;           //实例指针
};

#endif // CONFIG_H
