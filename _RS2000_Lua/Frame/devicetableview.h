#ifndef DEVICETABLEVIEW_H
#define DEVICETABLEVIEW_H

#include <QTableView>
#include <QItemDelegate>
#include <QMenu>
class QItemSelection;
class QItemSelectionModel;
class DeviceModel;
class AES70Device;

class ReadOnlyDelegate: public QItemDelegate
{
public:
    ReadOnlyDelegate(QWidget *parent = NULL):QItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override //final
    {
        Q_UNUSED(parent)
        Q_UNUSED(option)
        Q_UNUSED(index)
        return NULL;
    }
};

class DeviceTableView : public QTableView
{
    Q_OBJECT
public:
    DeviceTableView(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event) override;

public slots:    
    void removeDevice();
    void showOnlineDevices(bool isOnlineOnly);
    void deviceCurRoomNameChangeds(const QString& oldFilePath, const QString& newFilePath);
    void deleteDeviceCurRoomName(const QString& filePath);
    void openDeviceModulePath();
signals:

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void performDrag();
    AES70Device* getCurDev();

private:
    QPoint _startPos;
//    AES70Device* _dragDev;
    DeviceModel *_devicesModel;
    QMenu _contextMenu; //提供一个上下文菜单
    QAction *_delAction;
    QAction *_showOnlineAction;
    QAction *_openDevModulePath;
    ReadOnlyDelegate* _readOnlyDelegate;
};



#endif // DEVICETABLEVIEW_H
