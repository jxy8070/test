#include "stable.h"
#include "eqgraph.h"
#include "eqdragpoint.h"
#include "curvecalc.h"
#include "config.h"

EqGraph::EqGraph(QWidget *parent)
    :QGraphicsView(parent)
{
    _scale_cnt_special = 201;
    _curType = EQ_HIGHPASS;
    _scene = NULL;
    _graphsItem = NULL;
    _pathCurves = NULL;
    _pathArea = NULL;
    _pathCurrent = NULL;
    _pathCurrentHP = NULL;
    _pathCurrentLP = NULL;
    _curDragPoint = NULL;

    _isBypass = false;
    _isBypassHP = false;
    _isBypassLP = false;
    _isShowDrag = true;
    _isShowCurve = false;
    _invert = false;
    _passType = 0;
    _curIndex = -1;
    _fxAxisPoints<<30<<40<<50<<60<<70<<80<<90<<100<<200<<300<<400<<500<<600<<700<<800<<900<<1000
                <<2000<<3000<<4000<<5000<<6000<<7000<<8000<<9000<<10000;
    _margin = 25 + 1;

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示水平滚动条
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//视图默认消除边缘锯齿或者使用平滑像素图转换算法
    setFrameShape(QFrame::NoFrame);//不显示框架样式
    setMinimumSize(450,300);
}

EqGraph::EqGraph(QList<EqPara*> eqParaList, QWidget *parent)
    :QGraphicsView(parent)
{
    _scale_cnt_special = 201;
    _curType = EQ_HIGHPASS;
    _scene = NULL;
    _graphsItem = NULL;
    _pathCurves = NULL;
    _pathArea = NULL;
    _pathCurrent = NULL;
    _pathCurrentHP = NULL;
    _pathCurrentLP = NULL;
    _curDragPoint = NULL;
    _isBypass = false;
    _isBypassHP = false;
    _isBypassLP = false;
    _isShowDrag = true;
    _isShowCurve = false;
    _invert = false;
    _curIndex = -1;
    _passType = 0;
    _fxAxisPoints<<30<<40<<50<<60<<70<<80<<90<<100<<200<<300<<400<<500<<600<<700<<800<<900<<1000
                <<2000<<3000<<4000<<5000<<6000<<7000<<8000<<9000<<10000;
    _margin = 25 + 1;

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//屏蔽垂直方向的滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//屏蔽水平方向的滚动条
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//抗锯齿或者平滑像素图转换算法
    setFrameShape(QFrame::NoFrame);//不设置框架样式
    setMinimumSize(450,300);
    initEqParaList(eqParaList);
}

EqGraph::~EqGraph()
{
    qDeleteAll(_eqParaList);
}

void EqGraph::initParaList(int freq, int gain, int q, EqType type)
{
    EqPara *eqParaScale = new EqPara(freq, gain, q, type);

    _eqParaList.append(eqParaScale);
}

void EqGraph::initParaListEnd(void)
{
    for(int i = 0; i < _eqParaList.length(); i++) {
        float *eqResult = new float[X_LEN];//动态分配内存空间
        memset(eqResult, 0, X_LEN * sizeof(float));//初始化0
        _eqParaList[i]->result = eqResult;
    }

//Custom Fir Eq Graph Test Data
//    float tempCoeff[256] = {0.0002346480947792, 0.0003776113607488,-0.0019772284810982,-0.0055070984186730,-0.0075160215939921,-0.0110112890198855,-0.0073063487552592,-0.0000331549291840,0.0034317942214795,
//        0.0139865815689713,0.0136280969725155,0.0124161726493569,0.0220332692661935,0.0181582532500226,0.0160811993170567,0.0041171206487940,-0.0133370736729075,-0.0062913402764396,-0.0132645463827991,-0.0184623861889150,
//        -0.0173468462241289,-0.0329150221046254,-0.0227877708203978,-0.0224486509574620,-0.0341340895840856,-0.0180098150530229,-0.0166480589937734,-0.0172527938855448,-0.0223325905347922,-0.0350229605178648,-0.0274904079991085,
//        -0.0375416897718429,-0.0316539144578647,-0.0115658550072988,-0.0237036775538137,-0.0088956087093176,0.0073579512035247,-0.0012086257771629,0.0084672800228299,0.0055678166107675,-0.0026836825050996,
//        -0.0048856131236960,-0.0294376467408605,-0.0442966638068411,-0.0425254790478235,-0.0523823990813933,-0.0471035703717357,-0.0242350867723140,-0.0091370052968181,0.0175748061921438,0.0343966292251748,0.0567448903397624,
//        0.1055105317657685,0.1033477938339561,0.1112466035916484,0.1174053088144581,0.0561150082761388,0.0484010301302970,0.0041926844044600,-0.0496404028069891,-0.0308578414501513,-0.0992026788122376,-0.0705455996778040,
//        -0.0130084607327952,-0.0345445455175707,0.1018029006536788,0.1174555559298110,0.1589076625578250,0.3346931729240964,0.2458915450252603,0.3369944895010898,0.3615138190837527,0.0907908495136749,0.2620101527231126,
//        -0.0767989044498233,-0.7762055620729842,-0.0072612113804462,0.7098293120694792,0.0477150946633772,-0.2653564211458537,-0.1244024243992616,-0.2029897922995020,-0.0695323120846224,-0.0070982993133600,-0.0976547380568866,
//        -0.0190413272295519,0.0054477472296909,0.0006705834617182,0.0388455413321562,-0.0189836615053736,-0.0212374819060345,0.0913547179812880,0.0734032215800429,-0.0216865043244934,0.0216038259550192,0.0459884551969042,
//        0.0433830727770066,0.1093055989811919,0.0473943093034944,0.0212135266925725,0.1108215333201545,-0.0297386023849140,-0.1219139817307645,0.0940519279972701,0.1305326567583583,-0.0208912012948479,0.0105332193689693,
//        0.0205851224364348,-0.0774940660948352,-0.0438439010840956,0.0269239610616947,0.0283866236902717,0.0136955039161682,-0.0380254782144323,-0.0649521201231600,-0.0311821899405509,0.0115062729338032,0.0316828880629974,
//        0.0037491077309351,-0.0181709251515928,-0.0203537603477689,-0.0275781364474373,-0.0095130669043705,0.0052283219143706,0.0128560492966432,0.0182845221541585,0.0105170846237657,0.0146461066794788,-0.0089677515213271,
//        -0.0140427433271463,0.0329021556144677,0.0039003619223642,-0.0392439019194054,-0.0012344429162441,0.0021013450496254,-0.0326540139728427,-0.0157712526076496,0.0076916073483086,-0.0115613115814876,-0.0195383792138752,
//        -0.0139287987374665,-0.0370293491696050,-0.0292713126472816,0.0113256146634288,0.0098275194828263,0.0061191952209985,0.0203679344272605,0.0067108739419311,-0.0006798961698734,0.0172561842004658,0.0225415148218794,
//        0.0221913576698952,0.0188479536905423,0.0036114564433807,0.0006202396914625,-0.0031494056529780,-0.0114528637185020,-0.0056277697444079,-0.0184615708409484,-0.0294163205495621,-0.0124297633891007,-0.0125330719260727,
//        -0.0122163056820463,0.0052227016643261,0.0052898957457112,-0.0023097684497531,0.0049705943051235,0.0173386062624833,0.0096878347974643,-0.0003287226903281,-0.0005377190915393,-0.0183893291176314,-0.0212659087159267,
//        -0.0108932458119753,-0.0255558997345273,-0.0248347348705908,-0.0128517588154448,-0.0125266484564007,-0.0039512462787209,0.0000235399338000,0.0048529720821243,0.0148263682673262,0.0154262165640003,0.0168325886697686,
//        0.0098653876386708,-0.0065902978104579,-0.0178347341823883,-0.0290934496449364,-0.0395175594804199,-0.0478091402550300,-0.0529522582822740,-0.0538999228067268,-0.0521009775876467,-0.0380202733369352,-0.0180039928128752,
//        -0.0031457312172824,0.0154905227661485,0.0318838029011354,0.0366833656754284,0.0442294592398057,0.0489415452484209,0.0373208115949828,0.0261856773033378,0.0142377555904603,-0.0033327404732359,-0.0116631974232269,-0.0219783651187058,
//        -0.0369184467154476,-0.0459726262742519,-0.0478168491924236,-0.0318950966999483,-0.0089305276478741,0.0126589917539568,0.0334927985669102,0.0402200084630037,0.0417426426215719,0.0364142004688109,0.0228551428797573,
//        0.0161024114839487,0.0029868131852058,-0.0088465999624628,-0.0127548769392734,-0.0221929442997036,-0.0217128188754979,-0.0177765018093062,-0.0155216159142180,-0.0024546277056404,0.0057802122234448,0.0111494519922249,
//        0.0172725467048943,0.0163238224069868,0.0179763011896960,0.0180680176493106,0.0124515805820630,0.0020222715826765,-0.0162210234901849,-0.0312778899416762,-0.0420085698496642,-0.0518176112232673,-0.0556163756828170,
//        -0.0567952170749298,-0.0537269830163576,-0.0431491263286305,-0.0300158315345967,-0.0158796869467595,-0.0035567092366690,0.0024538379280250,0.0054391747521486,0.0066082223684222,0.0050308589834228,0.0036116928579741,
//                            0.0020505599382804,0.0005751358506896,0.0000607779128557,-0.0001092305126612,-0.0000225796002542
//};
//    for (int i = 0; i < 256; ++i) {
//        _customFirCoeff[i] = tempCoeff[i];
//    }
    memset(_customFirCoeff, 0, sizeof(_customFirCoeff));
    _customFirCoeff_Len = 256;

    _curType = _eqParaList.at(0)->type;
    if (_curType == EQ_NORMAL_CONFIG  || _curType == EQ_SHG_CONFIG) {
        _scale_cnt_special = 301;
    }
    else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG) {
        _scale_cnt_special = 401;
    }
    else {
        _scale_cnt_special = 201;
    }
}

void EqGraph::initEqParaList(QList<EqPara *> eqParaList)
{
    EqType type = eqParaList.at(0)->type;

    foreach (EqPara *tmp, eqParaList) {
        EqPara *eqParaScale = new EqPara(tmp->freq, tmp->gain, tmp->q, type);

        _eqParaList.append(eqParaScale);
    }
    initParaListEnd();
}

void EqGraph::calcEq(int idx)
{
    if(_scene == NULL || _eqParaList.length() == 0)
    {
        return;
    }

    CurveCalc::calc(_eqParaList.at(idx)->type,
                    _eqParaList.at(idx)->freq,
                    _eqParaList.at(idx)->gain / 100.,
                    _eqParaList.at(idx)->q / 100.,
                    _invert,
                    m_xFs_tab, X_LEN,
                    _customFirCoeff, _customFirCoeff_Len,
                    _eqParaList.at(idx)->result,
                    _passType);

    reflashPath();
}

void EqGraph::onGainChanged(int idx, int gain)
{
    setValue(idx, _eqParaList.at(idx)->freq, gain, _eqParaList.at(idx)->q);
}

void EqGraph::resetGain()
{
    for(int idx = 0; idx < _eqParaList.length(); idx++)
    {
        if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
        {
            _eqParaList.at(idx)->gain = 200;
        }
        else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
        {
            _eqParaList.at(idx)->gain = 300;
        }
        else
        {
            _eqParaList.at(idx)->gain = 100;
        }
        float *eqResult = _eqParaList.at(idx)->result;
        memset(eqResult, 0, X_LEN * sizeof(float));
        EqDragPoint* dragPoint= getDragPoint(idx);
        setRestrictRect(idx, dragPoint);
    }

    reflashPath();
}

void EqGraph::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    _curDragPoint = NULL;

    initGraph();
}

void EqGraph::calConstData()
{
    //geometry()函数保存小部件相对于其父类部件的几何图形
    _width = this->geometry().width()- 2 * _margin -2;
    _height = this->geometry().height() - 2 * _margin -2;

    _scale_fs = _width / 3.0;
    if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
    {
        _scale_gain = _height / 45.0;
    }
    else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
    {
        _scale_gain = _height / 40.0;
    }
    else if (_curType == EQ_ALLPASS_CONFIG) {
        _scale_gain = _height / (2 * PI);
    }
    else
    {
        _scale_gain = _height / 40.0;
    }

    memset(m_xFs_tab, 0, X_LEN* sizeof(float));
    memset(m_x_tab, 0, X_LEN* sizeof(float));
    float stepLG = X_LEN - 1;
    for(int i = 0; i < X_LEN; i++)
    {
        m_xFs_tab[i] = 20 * (pow(10.0, 3.0 * i / stepLG));
        m_x_tab[i] = i * _width/stepLG + _margin;
    }
}

int EqGraph::Float2Scale(float value, int type)
{
    int rs;
    switch (type) {
    case 0://fs
        rs = qRound(log10(value/20.0) *200 /3);//qRound()四舍五入
        break;
    case 1: //gain
        if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
        {
            rs = qRound(value * 20 / 3.0 +  2 * (_scale_cnt_special - 1) / 3);
        }
        else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
        {
            rs = qRound(value * 10.0 + 3.0 * (_scale_cnt_special - 1) / 4.0);
        }
        else
        {
            rs = qRound(value * 5 + (_scale_cnt_special - 1) / 2);
        }
        break;
    case 2: //q
        rs = qRound(log10(20* value) * (SCALE_CNT - 1)/2.6);
        break;
    default:
        Q_ASSERT(false);
    }
    restrictScale(rs);
    return rs;
}

float EqGraph::Scale2Float(int value, int type)
{
    if(!(value>=0 && value <= (_scale_cnt_special - 1)))
    {
        Q_ASSERT(value>=0 && value <= (_scale_cnt_special - 1));
    }
    float rs;
    switch (type) {
    case 0://fs
        rs = 20 * (pow(10.0, 3.0 * value / (SCALE_CNT - 1)));//pow(a,b); 求a的b次方
        break;
    case 1: //gain
        if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
        {
            rs = 3 / 20.0 * (value - 2 * (_scale_cnt_special - 1) / 3);
        }
        else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
        {
            rs = (value - 3 * (_scale_cnt_special - 1) / 4) / 10.0;
        }
        else
        {
            rs = 0.2 * (value - (_scale_cnt_special - 1) / 2);
        }
        break;
    case 2: //q
        rs = pow(10.0, 2.6 * value / (SCALE_CNT - 1)) / 20.0;
        break;
    default:
        Q_ASSERT(false);
    }
    return rs;
}

void EqGraph::setRestrictRect(int idx, EqDragPoint* dragPoint)
{
    Q_ASSERT(dragPoint);

    int fs_scale = EqGraph::Float2Scale(_eqParaList.at(idx)->freq, 0);
    int gain_scale = EqGraph::Float2Scale(_eqParaList.at(idx)->gain / 100., 1);
    float x = fs_scale * (_width-1) / (SCALE_CNT-1) + _margin;
    float y = _height - gain_scale * (_height -1) / (_scale_cnt_special-1) + _margin;
    dragPoint->setPos(x, y);

    switch (_eqParaList.at(idx)->type) {
    case EQ_NORMAL:
    case EQ_NORMAL_CONFIG:
    case EQ_HIGHSHELF:
    case EQ_LOWSHELF:
    case EQ_HIGHSHELF_CONFIG:
    case EQ_LOWSHELF_CONFIG:
    case EQ_CUSTOM_FIR_CONFIG:
        dragPoint->setRestrictRect(QRect(_margin, _margin, _width, _height));
        break;
    case EQ_SHG:
    case EQ_SHG_CONFIG:
        dragPoint->setRestrictRect(QRect(x, _margin, 1, _height));
        break;
    case EQ_HIGHPASS:
    case EQ_LOWPASS:
    case EQ_BANDPASS_H:
    case EQ_BANDPASS_L:
        dragPoint->setRestrictRect(QRect(_margin, _margin + _height/2, _width, 1));
        break;
    case EQ_HIGHPASS_CONFIG:
    case EQ_LOWPASS_CONFIG:
    case EQ_BANDPASS_CONFIG:
        dragPoint->setRestrictRect(QRect(_margin, _margin + _height/4, _width, 1));
        break;
    case EQ_ALLPASS_CONFIG:
        dragPoint->setRestrictRect(QRect(_margin, _margin + _height/2, _width, 1));
        break;
    default:
        break;
    }
}

void EqGraph::initGraph()
{
    if( _scene != NULL) {
        _scene->clear();
        delete _scene;
        _scene= NULL;
    }

    calConstData();
    _scene = new QGraphicsScene(this);
    _scene->setBackgroundBrush(QColor(50, 50, 50));//设置背景色
    _scene->setSceneRect(_margin, _margin, _width, _height);//设置场景的边界矩形
    setScene(_scene);

    _bypassText = new QGraphicsTextItem();
    _bypassText->setZValue(DRAG_ZVALUE);
    _bypassText->setDefaultTextColor(QColor(255, 0, 0));  // 文本色
    _bypassText->setPlainText("All Bypass");
    _bypassText->setFont(SkinStyle::getFont(32, true));
    _bypassText->setPos((width()-_bypassText->boundingRect().width())/2, (height()-_bypassText->boundingRect().height())/2);
    _scene->addItem(_bypassText);

    _graphsItem = new QGraphicsRectItem(_margin, _margin, _width, _height);
    _graphsItem->setZValue(GRID_ZVALUE);
    _graphsItem->setPen(QPen(QColor(178, 178, 178), 2));
    _graphsItem->setBrush(QBrush(QColor(50, 50, 50), Qt::SolidPattern));

    QPen linePen = QPen(QColor(178, 178, 178), 1, Qt::SolidLine);
    int xLineCnt = 8;
    if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
    {
        xLineCnt = 10;
    }
    else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
    {
        xLineCnt = 14;
    }
    else {
        xLineCnt = 8;
    }
    for(int i = 1; i < xLineCnt; i++)
    {
         float y = qRound(_margin + _height* 1.0 * i / xLineCnt) + 0.5;
         QGraphicsLineItem* xGridLine = new QGraphicsLineItem(QLineF(_margin, y,
                                _width + _margin, y),_graphsItem);
         xGridLine->setPen(linePen);
    }

    for(int i = 0; i < _fxAxisPoints.count(); i++)
    {
        float  x = qRound(log10(_fxAxisPoints[i] / 20.0) * _scale_fs + _margin) + 0.5;
        QGraphicsLineItem* yGridLine = new QGraphicsLineItem(
                    QLineF(x, _margin, x, _height + _margin),_graphsItem);
        yGridLine->setPen(linePen);
    }

    initAxis();

    _pathCurves = new QGraphicsPathItem();
    _pathCurves->setZValue(PATH_ZVALUE + 1);
    if (_curType == EQ_BANDPASS_H || _curType == EQ_BANDPASS_L)
    {
        _pathCurrentHP = new QGraphicsPathItem();
        _pathCurrentHP->setZValue(PATH_ZVALUE);
        _pathCurrentLP = new QGraphicsPathItem();
        _pathCurrentLP->setZValue(PATH_ZVALUE);
    }
    else
    {
        _pathCurrent = new QGraphicsPathItem();
        _pathCurrent->setZValue(PATH_ZVALUE);
    }
    _pathArea = new QGraphicsPathItem();
    _pathArea->setZValue(PATH_ZVALUE - 1);

    _scene->addItem(_graphsItem);
    _scene->addItem(_pathCurves);
    _scene->addItem(_pathArea);
    if (_curType == EQ_BANDPASS_H || _curType == EQ_BANDPASS_L)
    {
        _scene->addItem(_pathCurrentHP);
        _scene->addItem(_pathCurrentLP);
    }
    else
    {
        _scene->addItem(_pathCurrent);
    }

    //初始化EQ拖拽点
    Drag_Type type = Drag_Normal;
    if (_curType == EQ_BANDPASS_H || _curType == EQ_BANDPASS_L) {
        type = Drag_2Point_HP_LP;
    }
    for(int idx = 0; idx < _eqParaList.length(); idx++) {
        EqDragPoint* dragPoint = new EqDragPoint(this, idx, type);
        _eqParaList.at(idx)->dragPoint = dragPoint;
        if (_curType != EQ_CUSTOM_FIR_CONFIG) {
            dragPoint->setZValue(DRAG_ZVALUE);
            _scene->addItem(dragPoint);
        }
        setRestrictRect(idx, dragPoint);

        calcEq(idx);
    }

    _pointText = new QGraphicsTextItem();
    _pointText->setZValue(DRAG_ZVALUE);
    _pointText->setDefaultTextColor(QColor(255, 255, 255));  // 文本色
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = _pointText->textCursor();
    cursor.mergeBlockFormat(format);
    _pointText->setTextCursor(cursor);

    if(_curIndex >=0 || _curIndex< _eqParaList.count())
    {
        setCurrentDragPoint(_curIndex);
    }

    for (int i = 0; i < _eqParaList.count(); ++i)
    {
        _curIndex = i;
        reflashPath();
    }
}

void EqGraph::initAxis()
{
    int yAxisCnt = 5;
    int yStart = 20;
    int yStep = 10;
    if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
    {
        yAxisCnt = 10;
        yStart = 15;
        yStep  = 5;
    }
    else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
    {
        yAxisCnt = 7;
        yStart   = 6;
        yStep    = 6;
    }
    for(int i=0; i<yAxisCnt; i++)
    {
        QString yTextL = QString::number(yStart-i*yStep);
        QString yTextR = "  " + QString::number(yStart-i*yStep);    //实现右侧db值文本右对齐效果
        yTextR = yTextR.right(3);

        int y = _margin + _height*i/(yAxisCnt -1);
        QGraphicsTextItem* leftText = new QGraphicsTextItem(yTextL, _graphsItem);
        leftText->setPos(2,y-TEXT_HIGH/2);
        leftText->setDefaultTextColor(QColor(255, 255, 255));
        if (_curType == EQ_ALLPASS_CONFIG) {
            QString rightyText = QString::number(180-i*90);
            QGraphicsTextItem* rightText = new QGraphicsTextItem(rightyText, _graphsItem);
            rightText->setPos(_width + _margin, y-TEXT_HIGH/2);
            rightText->setDefaultTextColor(QColor(255, 255, 255));
        }
        else {
            QGraphicsTextItem* rightText = new QGraphicsTextItem(yTextR, _graphsItem);
            rightText->setPos(_width + _margin, y-TEXT_HIGH/2);
            rightText->setDefaultTextColor(QColor(255, 255, 255));
        }
    }
    QList<float> xAxis;
    xAxis <<20<<40<<100<<200<<400<<1000<<2000<<4000<<10000<<20000;
    for(int i = 0; i< xAxis.count(); i++)
    {
        float fx = xAxis[i];
        float  x = log10(fx / 20.0) * _scale_fs + _margin;
        QString xText;
        if(fx<1000) xText = QString::number(fx);
        else
        {
            xText = QString::number(fx/1000.0, 'f', 0) + "k";
        }
        QGraphicsTextItem* downText = new QGraphicsTextItem(xText, _graphsItem);
        downText->setPos(x-12, _height + _margin +2);
        downText->setDefaultTextColor(QColor(255, 255, 255));
    }
}

void EqGraph::reflashPath()
{
    if( _scene == NULL) return;

    QPainterPath curves = QPainterPath();//为绘制操作提供一个容器
    QPainterPath area = QPainterPath();
    QPainterPath current = QPainterPath();
    int curvesPenW = 2;

    qreal dx = 1.5;
    if (_curType == EQ_BANDPASS_H || _curType == EQ_BANDPASS_L) {
        float sum_tab[X_LEN];
        memset(sum_tab, 0, X_LEN*sizeof(float));

        int jstart = 0;
        int jend = _eqParaList.count();
        if (_isBypassHP && !_isBypassLP)
        {
            jstart = 1;
        }
        else if (!_isBypassHP && _isBypassLP)
        {
            jend = 1;
        }
        int y0; //第一个点的y值
        for(int i = 0; i < X_LEN; i++)
        {
            for(int j = jstart; j < jend; j++)
            {
                if (_eqParaList.at(j)->bypass == true) continue;

                sum_tab[i] += _eqParaList.at(j)->result[i];
            }
            restrictGain(sum_tab[i]);
            int yi = _height / 2.0 + _margin - _scale_gain * sum_tab[i];
            if(i==0)
            {
                y0 = yi;
                area.moveTo(m_x_tab[0] + dx, yi);
                //修正线宽导致曲线画出框外的问题
                curves.moveTo(m_x_tab[0] + dx, yi);
            }
            else if(i == X_LEN-1)
            {
                curves.lineTo(m_x_tab[X_LEN-1] - dx, yi);//从当前位置向给定位置添加一条线
                area.lineTo(m_x_tab[X_LEN-1]- dx, yi);
                area.lineTo(m_x_tab[X_LEN-1]- dx, _height + _margin);
                area.lineTo(m_x_tab[0] + dx, _height + _margin);
                area.lineTo(m_x_tab[0] + dx, y0);
            }
            else
            {
                curves.lineTo(m_x_tab[i], yi);
                area.lineTo(m_x_tab[i], yi);
            }
            if(_curIndex != -1)
            {
                restrictGain(_eqParaList.at(_curIndex)->result[i]);
                int yc = _height / 2.0 + _margin - _scale_gain * _eqParaList.at(_curIndex)->result[i];
                if(i == 0)
                {
                    current.moveTo(m_x_tab[0] + dx, yc);
                }
                else if(i == X_LEN-1)
                {
                    current.lineTo(m_x_tab[X_LEN-1] -dx, yc);
                }
                else
                {
                    current.lineTo(m_x_tab[i], yc);
                }
            }
        }

        _pathCurves->setPath(curves);
        _pathArea->setPath(area);

        switch (_curIndex)
        {
        case 0:
            _pathCurrentHP->setPath(current);//以QPainterPath形式返回项的路径
            break;
        case 1:
            _pathCurrentLP->setPath(current);
            break;
        default:
            break;
        }

        _pathCurrentHP->setPen(QPen(QColor(255, 255, 255), curvesPenW));
        _pathCurrentLP->setPen(QPen(QColor(255, 255, 255), curvesPenW));

        if(_isBypassHP & _isBypassLP)//bypass关闭
        {
            _pathCurves->setPen(QPen(QColor(80, 80, 80), curvesPenW));
            _pathArea->setBrush(QBrush(QColor(178, 178, 178, 128), Qt::SolidPattern));
            _pathArea->setPen(QPen(QColor(178, 178, 178, 128), 1));
        }
        else//bypass开启
        {
            _pathCurves->setPen(QPen(QColor(60, 192, 197), curvesPenW));
            _pathArea->setBrush(QBrush(QColor(60, 192, 197, 90), Qt::SolidPattern));
            _pathArea->setPen(QPen(QColor(60, 192, 197, 90), 1));
        }
    }
    else {
//        qDebug()<<__FUNCTION__<<__LINE__<<_curType;
        float sum_tab[X_LEN];
        float upAndDownRatio = 2.0;
        if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
        {
            upAndDownRatio = 3.0;
        }
        else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
        {
            upAndDownRatio = 4.0;
        }
        memset(sum_tab, 0, X_LEN*sizeof(float));

        int y0; //第一个点的y值
        for(int i = 0; i < X_LEN; i++)
        {
            for(int j = 0; j < _eqParaList.count(); j++)
            {
                if (_eqParaList.at(j)->bypass == true) continue;
                sum_tab[i] += _eqParaList.at(j)->result[i];
            }
            restrictGain(sum_tab[i]);
            int yi = _height / upAndDownRatio + _margin - _scale_gain * sum_tab[i];
            if(i==0)
            {
                y0 = yi;
                curves.moveTo(m_x_tab[0] + dx, yi); //修正线宽导致曲线画出框外的问题
                area.moveTo(m_x_tab[0] + dx, yi);
            }
            else if(i == X_LEN-1)
            {
                curves.lineTo(m_x_tab[X_LEN-1] - dx, yi);
                area.lineTo(m_x_tab[X_LEN-1] - dx, yi);
                if (_curType == EQ_HIGHPASS || _curType == EQ_LOWPASS || _curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_BANDPASS_CONFIG)
                {
//                    qDebug()<<__FUNCTION__<<__LINE__<<_curType;
                    area.lineTo(m_x_tab[X_LEN-1] - dx, _height + _margin);
                    area.lineTo(m_x_tab[0]+ dx, _height + _margin);
                }
                else
                {
//                    qDebug()<<__FUNCTION__<<__LINE__<<_curType;
                    area.lineTo(m_x_tab[X_LEN-1] - dx, _height / upAndDownRatio + _margin);
                    area.lineTo(m_x_tab[0] + dx, _height / upAndDownRatio + _margin);
                }
                area.lineTo(m_x_tab[0] + dx, y0);
            }
            else
            {
                curves.lineTo(m_x_tab[i], yi);
                area.lineTo(m_x_tab[i], yi);
            }
            if(_curIndex!=-1)
            {
                restrictGain(_eqParaList.at(_curIndex)->result[i]);
                int yc = _height / upAndDownRatio + _margin - _scale_gain * _eqParaList.at(_curIndex)->result[i];
                if(i == 0) current.moveTo(m_x_tab[0] + dx, yc);
                else if(i == X_LEN-1) current.lineTo(m_x_tab[X_LEN-1] -dx, yc);
                else current.lineTo(m_x_tab[i], yc);
            }
        }
        _pathCurves->setPath(curves);
        if (_curType != EQ_CUSTOM_FIR_CONFIG) {
            _pathArea->setPath(area);
        }
        if(_curIndex!=-1) _pathCurrent->setPath(current);//以QPainterPath形式返回项的路径

        _pathCurrent->setPen(QPen(QColor(255, 255, 255), curvesPenW));

        if(_isBypass)
        {
            _bypassText->setVisible(true);
            _pathCurrent->setVisible(false);
            _pathCurves->setPen(QPen(QColor(80, 80, 80), curvesPenW));
            _pathArea->setBrush(QBrush(QColor(178, 178, 178, 128), Qt::SolidPattern));
            _pathArea->setPen(QPen(QColor(178, 178, 178, 128), 1));
        }
        else
        {
            _bypassText->setVisible(false);
            _pathCurrent->setVisible(true);
            _pathCurves->setPen(QPen(QColor(60, 192, 197), curvesPenW));
            _pathArea->setBrush(QBrush(QColor(60, 192, 197, 90), Qt::SolidPattern));
            _pathArea->setPen(QPen(QColor(60, 192, 197, 90), 1));
        }
    }
}

void EqGraph::setBypass(bool isBypass)
{
    _isBypass = isBypass;
    reflashPath();
}

void EqGraph::setHPLPBypass(bool HPBypass, bool LPBypass)
{
    _isBypassHP = HPBypass;
    _isBypassLP = LPBypass;
    reflashPath();
}

void EqGraph::setShowDrag(bool isShowDrag)
{
    _isShowDrag = isShowDrag;
    update();
}

void EqGraph::setShowCurve(bool isShowCurve)
{
    _isShowCurve = isShowCurve;
    reflashPath();
}

void EqGraph::setCurrentDragPoint(EqDragPoint* dragPoint)
{
    Q_ASSERT(dragPoint);

    if(dragPoint != _curDragPoint)
    {
        dragPoint->setZValue(ACTIVE_DRAG_ZVALUE);//设置Z值,高Z值堆叠在低Z值之上
        dragPoint->setIsCurrent(true);
        if(_curDragPoint != NULL)
        {
            _curDragPoint->setZValue(DRAG_ZVALUE);
            _curDragPoint->setIsCurrent(false);
        }
        _curDragPoint = dragPoint;
        _curIndex = dragPoint->getIndex();

        /* 切换操作点时不发送数据 */
        emit selectPointChanged(_curIndex);
//        emit valueChanged(_curIndex,  _eqParaList.at(_curIndex)->freq,
//                          _eqParaList.at(_curIndex)->gain, _eqParaList.at(_curIndex)->q);
    }
}

void EqGraph::setValue(int idx, int fs, int gain, int q)
{
//    qDebug() << __FUNCTION__ << __LINE__  << idx << fs << gain << q;
    int fs_scale = EqGraph::Float2Scale(fs, 0);
    int gain_scale = EqGraph::Float2Scale(gain / 100., 1);

    setCurrentDragPoint(idx);
    EqDragPoint* dragPoint = getDragPoint(idx);
    if(dragPoint != NULL){
        float x = fs_scale * (_width-1) / (SCALE_CNT-1) + _margin;
        float y = _height - gain_scale * (_height -1) /(_scale_cnt_special-1) + _margin;
        dragPoint->setPosAndRestrict(x, y);//设置Eq小圆点的位置
    }else {
//        qDebug()<<__FUNCTION__<<__LINE__;
    }
    _eqParaList.at(idx)->freq = fs;
    _eqParaList.at(idx)->gain = gain;
    _eqParaList.at(idx)->q = q;
    calcEq(idx);
}

void EqGraph::setSegBypass(int idx, bool enable)
{
    _eqParaList.at(idx)->bypass = enable;
    calcEq(idx);
}

void EqGraph::setSegInvert(int idx, bool enable)
{
    _invert = enable;
    calcEq(idx);
}

void EqGraph::setType(int idx, EqType type)
{
    setCurrentDragPoint(idx);
    _eqParaList.at(idx)->type= type;
    calcEq(idx);
}

void EqGraph::setCoeff(float *coeff, int len)
{
    for (int i = 0; i < len; ++i) {
        _customFirCoeff[i] = coeff[i];
//        qDebug() << __FUNCTION__ << __LINE__ << i << len << _customFirCoeff[i] << coeff[i];
    }
    _customFirCoeff_Len = len;
    calcEq(0);
}

void EqGraph::eqParaChanged(int idx, int posX, int posY)
{
//    qDebug() << __FUNCTION__ << __LINE__ << _curType << _scale_cnt_special;
    int fs_scale = (posX - _margin)* (SCALE_CNT -1) / (_width -1);
    int gain_scale = (_height + _margin - posY) * (_scale_cnt_special -1) / (_height -1);
    restrictScale(fs_scale);
    restrictScale(gain_scale);

    int freq = Scale2Float(fs_scale, 0);
    int gain = Scale2Float(gain_scale, 1) * 100;
    if(_eqParaList.at(idx)->gain == gain && _eqParaList.at(idx)->freq == freq)
    {  //没有变化（在SCALE_CNT网格内）
       return;
    }

    _eqParaList.at(idx)->freq = freq;
    _eqParaList.at(idx)->gain = gain;


    calcEq(idx);

    emit valueChanged(idx, _eqParaList.at(idx)->freq, _eqParaList.at(idx)->gain, _eqParaList.at(idx)->q, _eqParaList.at(idx)->type);
    emit valueChanged(idx, _eqParaList.at(idx)->freq, _eqParaList.at(idx)->gain, _eqParaList.at(idx)->q);
}

void EqGraph::dispPointLabel(bool flag, int idx, QPointF pos)
{
    if (flag) {
        setPointLabelPos(idx, pos);
        _scene->addItem(_pointText);
    }
    else {
        _scene->removeItem(_pointText);
    }
}

void EqGraph::setPointLabelPos(int idx, QPointF pos)
{
    float freq = _eqParaList.at(idx)->freq;
    float gain = _eqParaList.at(idx)->gain / 100.;
    float q    = _eqParaList.at(idx)->q / 100.;
    QString display;
    switch (_curType) {
    case EQ_NORMAL_CONFIG:
        if (freq > 1000) display = QString("%1kHz\n").arg(QString::number(freq/1000.0, 'f', 3));
        else             display = QString("%1Hz\n").arg(QString::number(freq, 'f', 0));
        display += QString("%1dB\n").arg(QString::number(gain, 'f', 1));
        display += QString("%1").arg(QString::number(q, 'f', 2));
        break;
    case EQ_SHG_CONFIG:
//        if (freq > 1000) display = QString("%1kHz\n").arg(QString::number(freq/1000.0, 'f', 3));
//        else             display = QString("%1Hz\n").arg(QString::number(freq, 'f', 0));
        display += QString("%1dB\n").arg(QString::number(gain, 'f', 1));
//        display += QString("%1").arg(QString::number(q, 'f', 2));
        break;
    default: break;
    }

    _pointText->setPlainText(display);
    if (gain > 0) {
        _pointText->setPos(pos.x()-_pointText->sceneBoundingRect().width()/2, pos.y());
    }
    else {
        _pointText->setPos(pos.x()-_pointText->sceneBoundingRect().width()/2, pos.y()-_pointText->sceneBoundingRect().height());
    }
}

void EqGraph::restrictGain(float &value)
{
    if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
    {
        if(value > 15) value = 15;
        else if(value < -30) value = -30;
    }
    else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
    {
        if (value > 9) value = 9;
        else if (value < -30) value = -30;
    }
    else if (_curType == EQ_NORMAL_CONFIG) {
        if (value > PI) value = PI;
        else if (value < -PI) value = -PI;
    }
    else
    {
        if(value > 20) value = 20;
        else if(value < -20) value = -20;
    }
}

void EqGraph::restrictScale(int &value)
{
    if (_curType == EQ_NORMAL_CONFIG || _curType == EQ_SHG_CONFIG)
    {
        if (value > 300) value = 300;
        else if (value < 0) value = 0;
    }
    else if (_curType == EQ_HIGHPASS_CONFIG || _curType == EQ_LOWPASS_CONFIG || _curType == EQ_HIGHSHELF_CONFIG || _curType == EQ_LOWSHELF_CONFIG || _curType == EQ_BANDPASS_CONFIG)
    {
        if (value > 400) value = 400;
        else if (value < 0) value = 0;
    }
    else
    {
        if(value > 200) value = 200;
        else if(value < 0) value = 0;
    }
}


EqDragPoint* EqGraph::getDragPoint(int idx)
{
    if(_scene == NULL )
    {
        return NULL;
    }

    if(idx <0 || idx >_eqParaList.count())
    {
        return NULL;
    }
    return _eqParaList.at(idx)->dragPoint;
}

void EqGraph::setCurrentDragPoint(int idx)
{
    if(_curIndex != idx)
    {
        _curIndex = idx;
    }

    EqDragPoint* dragPoint = getDragPoint(idx);
    if(dragPoint != NULL)
    {
        setCurrentDragPoint(dragPoint);
    }
}

float EqGraph::getDistance(QPointF point1, QPointF point2)
{
    Q_ASSERT(false);
    return qSqrt(qPow((point1.x() -point2.x()),2) + qPow((point1.y() -point2.y()),2));
}
