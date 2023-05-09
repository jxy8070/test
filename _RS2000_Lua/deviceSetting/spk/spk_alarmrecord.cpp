#include "stable.h"
#include "spk_alarmrecord.h"


AlarmRecord::AlarmRecord(QString mac, QWidget *parent) :
    DevSettingDlg(parent)
{
    _devMac = mac;
    setupUi(mac);

    setWindowFlags(Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    setWindowTitle(mac);
    setMinimumSize(700,400);
    setStyleSheet(QLatin1String("QLabel{qproperty-alignment: 'AlignVCenter | AlignRight';}"));
    onSearch();
}



void AlarmRecord::setupUi(QString deviceMac)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *searchLayout = new QHBoxLayout();
    mainLayout->addLayout(searchLayout);

    QLabel *lblContext = new QLabel(tr("AlarmInfo"), this);
    searchLayout->addWidget(lblContext, 1);
    _txtContext = new QLineEdit(this);
    searchLayout->addWidget(_txtContext, 2);
    QDateTime todayStart;
    todayStart.setDate(QDateTime::currentDateTime().date());
    QDateTime todayEnd = todayStart.addDays(1).addSecs(-1);
    QLabel *lblStartTime = new QLabel(tr("StartTime"), this);
    searchLayout->addWidget(lblStartTime, 2);
    _timeStart = new QDateTimeEdit(this);
    _timeStart->setDateTime(todayStart);
    _timeStart->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    searchLayout->addWidget(_timeStart, 2);
    QLabel *lblEndTime = new QLabel(tr("EndTime"), this);
    searchLayout->addWidget(lblEndTime, 2);
    _timeEnd = new QDateTimeEdit(this);
    _timeEnd->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    _timeEnd->setDateTime(todayEnd);
    searchLayout->addWidget(_timeEnd, 2);
    QPushButton *btnSearch = new QPushButton(tr("search"), this);
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(onSearch()));
    searchLayout->addWidget(btnSearch, 1);

    _model =new QSqlTableModel(this);
    _model->setTable("alarmInfo");

    _model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    _model->setHeaderData(1, Qt::Horizontal, tr("AlarmTime"));
    _model->setHeaderData(2, Qt::Horizontal, tr("deviceMac"));
    _model->setHeaderData(3, Qt::Horizontal, tr("Channel"));
    _model->setHeaderData(4, Qt::Horizontal, tr("AlarmInfo"));
    QTableView *tableView = new QTableView();
    tableView->setSortingEnabled(true);
    tableView->setModel(_model);
    tableView->setColumnHidden(0,true);
    tableView->horizontalHeader()->setVisible(true);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(tableView);

    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}



void AlarmRecord::onSearch()
{
    QString filter;
    filter.append(QString("deviceMac= '%1'").arg(_devMac));
    filter.append(QString(" and alarmInfo like '%%1%' ").arg(_txtContext->text()));
    filter.append(QString(" and createTime > '%1' and createTime< '%2' ")
                  .arg(_timeStart->text()).arg(_timeEnd->text()));
    _model->setFilter(filter);
    _model->select();
}
