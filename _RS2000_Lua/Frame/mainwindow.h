#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "config.h"
#include "propertywidget.h"
#include "modeldockwidget.h"
#include "customWidget/noticewidget.h"

class BorderDock;
class ProjectView;
class AbsGraphView;
class ConfigScene;
class DeviceTableView;
class DeviceItem;
class MonitorWindows;
class UserControlInterfaceView;
class ResourceManager;
class SolutionDockWidget;
//主窗口
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
public:
    QMdiArea* getMdiArea(){ return _mdiArea; }
    void updateDeviceModeStatus(ConfigScene* cfgScene);
    void initDockWidgets();
    void updateDockWidgets(SubWnd_Type viewType, ConfigScene *cfgScene);
    void updateDockWidgets_Fangtu(SubWnd_Type viewType, ConfigScene *cfgScene);
    void updateDockWidgets_Others(SubWnd_Type viewType, ConfigScene *cfgScene);
    void updateMenuToolbars(SubWnd_Type viewType, ConfigScene *cfgScene);
    void setUsagePercent(float spacePercent, float timePercent);
    void initTreeProperty(QGraphicsItem* item);
    QString getSolutionPath(QString fileName = QString());
    template<typename View>
    View *getSubWindowForTitle(QString title);
    void openDevModulePath(QString Mac, QString fileName);
    QList<ConfigItem*> getCfgItemListOfCurWindow();
    void showNotice(QString notice, int second = 2000);

signals:
    void needInsertAlarm(QString,int,QString);
    void onSkinChanged();

public slots:    
    void openWelcomeDlg();
    void openSolution(QString slnPath);
    void reflashSolution();
    void openSubWindow(const QString& filePath);
    void deleteSubWindow(const QString& filePath);
    void renameSubWindow(const QString& oldFilePath, const QString& newFilePath);
    void fileNewClicked(void);

    void setSheetSize(QRectF rect);
    void addUserControlInterfaceView(QGraphicsItem *item, QString name);
    void delUserControlInterfaceView(QGraphicsItem *item);
    void setCurrentSubWindow_UCI(QGraphicsItem *item, QString name);
    void onToolBtnCheckedSlot(ConfigScene::ToolkitType type);
    void setModelDockBoxToolkitBtnStatus(bool status);
    void updateSolutionRoomPorts(QString fileName, QMap<int, QList<QString>> roomInports, QMap<int, QList<QString>> roomOutports);
private slots:
    //文件菜单
    void fileImportClicked(void);
    void fileExportClicked(void);
    void onSubWindowClosed(void);
    void onSubWindowSaved(void);
    void saveAllSubWindow(void);
    void closeAllSubWindow(void);
    void fileSaveClicked(void);
    void fileSaveToDeviceClicked(void);
    void loadFromDeviceClicked(void);
    void fileCloseClicked(void);
    void resourceManagerClicked(void);

    //窗口菜单
    void subWindowShowMaximize();
    //对齐控件
    void setItemsAlign();
    //加载参数
    void loadAllSpeadkerConfig();
    void loadAllAmplifierConfig();
    void loadAllControllerConfig();
    //Help菜单
    void helpAbout();
    void helpDocuments();

    void onResetViews();
    void viewDeviceLineItem();
    void settingUserLockClicked(void);
    void settingUserManageClicked();
    void settingUserPasswordClicked();
    void clearOfflineDevice();
    void openCtrlCenterUtil();
    void setLanguage();
    void setSkinStyle();
    void downloadRouter();
    void downloadAlgorithm();
    void setAudioMute(bool isMute);
    void onSubWindowActivated(QMdiSubWindow *activeWindow);
    //设计窗口缩放
    void onZoomView_slots();
    void onViewScaleValueChanged_slot(qreal ScaleValue);
    void mouseStateSet(bool enable);
    //防火墙设置
    void Firewall_Settings();

private:
    void hadeMenuToolbars();
    void initStatusBar();

private:
    QUndoGroup *_undoGroup;
    QAction *_actionUndo;
    QAction *_actionRedo;
    QList<QLabel *> _statusLabelList;
    SubWnd_Type _lastViewType;

    SolutionDockWidget* _slnDockBox;    //解决方案
    DockWidget* _logsDockBox;           //运行日志
    DockWidget* _dbDevDockBox;          //数据库保存及网络发现的设备
    DockWidget* _devTypeDockBox;        //按类型分类的设备
    DockWidget* _devModelDockBox;       //设备固定模块，输入输出等
    ModelDockWidget* _modelDockBox;     //算法模块
    DockWidget* _pinsViewerDockBox;     //模块开放的控制信息
    PropertyWidget* _propertyDockBox;   //属性
    BorderDock *_toolBarL;
    BorderDock *_toolBarR;

    QList<QMdiSubWindow *> _uciSubWindowsList;
    ResourceManager *_resourceManager = NULL;
    QMdiSubWindow * _currentWin = NULL;
//////////////////////////////////////////////////
/// 实现Singleton模式相关代码
public:
    static MainWindow *instance();
private:
    MainWindow();//禁止构造函数。
    MainWindow(const MainWindow&);//禁止拷贝构造函数。
    MainWindow & operator=(const MainWindow&);//禁止赋值拷贝函数。
    static QMutex Mutex;//实例互斥锁。
    static MainWindow* _Instance; //<使用原子指针,默认初始化为0。
    enum {
        STATUSBAR_STATE = 0,
        STATUSBAR_DEVICE_STATUS,
        STATUSBAR_SHEET_SIZE,
        STATUSBAR_SPACE_USAGE,
        STATUSBAR_TIME_USAGE,
        STATUSBAR_SCALING,
        STATUSBAR_START_IP,
        STATUSBAR_END_IP,
        STATUSBAR_ADAPTER_IP,
        STATUSBAR_COUNT
    };
};




#endif // MAINWINDOW_H
