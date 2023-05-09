#include "stable.h"

#include "usermanagement.h"
#include "newuser.h"
#include "changepassword.h"
#include "config.h"


UserManagement::UserManagement(QWidget *parent) :
    QDialog(parent),
    m_newUser(new NewUser),
    m_changePassword(new ChangePassword)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & ~(Qt::WindowContextHelpButtonHint));
    connect(m_newUser, SIGNAL(addNewUser()), this, SLOT(add_new_user()));

    adminTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    adminTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    normalTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    normalTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    viewTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    viewTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    adminTableWidget->setAttribute(Qt::WA_StyledBackground);
//    viewTableWidget->setAttribute(Qt::WA_StyledBackground);
//    normalTableWidget->setAttribute(Qt::WA_StyledBackground);
#if Logo_FangTu
    setStyleSheet(QLatin1String(
                  "QTableWidget *{ background-color: rgba(78,82,92,255); }"
                  "QTabBar { "
                  "     border: 1px solid transparent; "
                  "     background: transparent; "
                  "     color: transparent; "
                  "     padding-bottom: 0px;"
                  "}"
                  "QTabBar::tab {\
                      border: 1px solid rgb(69,156,99);\
                      border-top-left-radius: 4px;\
                      border-top-right-radius: 4px;\
                      background: transparent;\
                      height: 20px;\
                      min-width: 50px;\
                      color: rgb(69,156,99);\
                      padding-bottom: 1px;\
                  }"
                  "QTabBar::tab::selected {"
                  "    border: 1px solid rgb(69,156,99);"
                  "    background: rgba(78,82,92,255);"
                  "    height: 22px;"
                  "    min-width: 50px;"
                  "    color: rgb(69,156,99);"
                  "    padding-bottom: 1px;"
                  "}"
                  "QTabBar::tab:hover { height: 22px; }"
                  "QTabBar::tab:!selected { margin-top: 2px; }"
                  "QTabWidget::pane {\
                          padding-top: 0px;\
                          border: 1px solid rgb(69,156,99);\
                          background: transparent;\
                          position: absolute;\
                          top: 0em;\
                  }"
                  "QHeaderView::section {"
                                      "color: white;"
                                      "background: rgba(78,82,92,255);"
                                    "}"
                                    "QTableView {\
                                        color: white;\
                                        gridline-color: rgb(105, 106, 110);\
                                        background: rgb(53, 57, 66);\
                                        selection-color: white;\
                                        selection-background-color: rgb(53, 53, 53);\
                                        border: 0px;\
                                        border-radius: 0px;\
                                    }"
                                        ));

#endif
}

UserManagement::~UserManagement()
{
//    qDebug("~UserManagement");
    clear();
}

void UserManagement::tableItemAdd(QString type)
{
    if (type == "Admin")
    {
        int rows = adminTableWidget->rowCount();
        adminTableWidget->insertRow(rows);

        adminTableWidget->setItem(rows, 0, _tableItem.last());

        adminTableWidget->item(rows, 0)->setTextAlignment(Qt::AlignCenter);
    }
    else if (type == "Normal")
    {
        int rows = normalTableWidget->rowCount();
        normalTableWidget->insertRow(rows);

        normalTableWidget->setItem(rows, 0, _tableItem.last());

        normalTableWidget->item(rows, 0)->setTextAlignment(Qt::AlignCenter);
    }
    else if (type == "Viewer")
    {
        int rows = viewTableWidget->rowCount();
        viewTableWidget->insertRow(rows);

        viewTableWidget->setItem(rows, 0, _tableItem.last());

        viewTableWidget->item(rows, 0)->setTextAlignment(Qt::AlignCenter);
    }
}

void UserManagement::tableItemRemove(QString type, QString name)
{

    for (int i = 0; i < Config::instance()->getUserList().count(); ++i)
    {
        if ((Config::instance()->getUserList().at(i).type == type) &&
                (Config::instance()->getUserList().at(i).name == name))
        {
//            Config::instance()->getUserList().removeAt(i);
            Config::instance()->deleteUser(i);
            delete _tableItem.value(i);
            _tableItem.removeAt(i);
            return;
        }
    }
}

void UserManagement::clear()
{
    qDeleteAll(_tableItem);
    _tableItem.clear();

    for (int i = 0; i < adminTableWidget->rowCount();)
    {
        adminTableWidget->removeRow(i);
    }
    for (int i = 0; i < normalTableWidget->rowCount();)
    {
        normalTableWidget->removeRow(i);
    }
    for (int i = 0; i < viewTableWidget->rowCount();)
    {
        viewTableWidget->removeRow(i);
    }
}

void UserManagement::init()
{
    foreach (UserInfo temp, Config::instance()->getUserList())
    {
        _tableItem.append(new QTableWidgetItem(temp.name));
        tableItemAdd(temp.type);
    }
}

void UserManagement::add_new_user()
{
    _tableItem.append(new QTableWidgetItem(Config::instance()->getUserList().last().name));
    tableItemAdd(Config::instance()->getUserList().last().type);
    Config::instance()->addNewUser();
}

void UserManagement::showEvent(QShowEvent *event)
{
    init();
    QDialog::showEvent(event);
}

void UserManagement::closeEvent(QCloseEvent *event)
{
//    Config::instance()->saveUserInfo();
    clear();
    QDialog::closeEvent(event);
}

void UserManagement::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }

    QDialog::keyPressEvent(event);
}


void UserManagement::on_newuser_clicked()
{
    if (Config::instance()->hasAdminPriv())
    {
        m_newUser->showWidget(tabWidget->currentIndex());
    }
}

void UserManagement::on_deleteuser_clicked()
{
    if (!Config::instance()->hasAdminPriv()){
        return;
    }

    QList<QTableWidgetItem *>selected;
    QString text;
    int row;

    switch (tabWidget->currentIndex()) {
    case 0:
        selected = viewTableWidget->selectedItems();
        if (selected.isEmpty())
        {
            break;
        }

        text = selected.last()->text();
        row = selected.last()->row();
        tableItemRemove("Viewer", text);
        viewTableWidget->removeRow(row);
        break;
    case 1:
        selected = normalTableWidget->selectedItems();
        if (selected.isEmpty())
        {
            break;
        }

        text = selected.last()->text();
        row = selected.last()->row();
        tableItemRemove("Normal", text);
        normalTableWidget->removeRow(row);
        break;
    case 2:
        selected = adminTableWidget->selectedItems();
        if (selected.isEmpty())
        {
            break;
        }

        text = selected.last()->text();
        row = selected.last()->row();
        tableItemRemove("Admin", text);
        adminTableWidget->removeRow(row);
        break;
    default:
        break;
    }
}

void UserManagement::on_password_clicked()
{
    if (!Config::instance()->hasAdminPriv()){
        return;
    }

    QList<QTableWidgetItem *>selected;
    QString tp;
    QString nm;
    QString ps;

    switch (tabWidget->currentIndex()) {
    case 0:
        selected = viewTableWidget->selectedItems();
        if (selected.isEmpty()) {
            return;
        }

        tp = "Viewer";
        break;
    case 1:
        selected = normalTableWidget->selectedItems();
        if (selected.isEmpty()) {
            return;
        }

        tp = "Normal";
        break;
    case 2:
        selected = adminTableWidget->selectedItems();
        if (selected.isEmpty()) {
            return;
        }

        tp = "Admin";
        break;
    default:
        break;
    }

    nm = selected.last()->text();
    foreach (UserInfo info, Config::instance()->getUserList())
    {
        if (info.type == tp && info.name == nm)
        {
            ps = info.password;
        }
    }

    m_changePassword->setChangePasswordInfo(tp, nm, ps);
    m_changePassword->show();
}
