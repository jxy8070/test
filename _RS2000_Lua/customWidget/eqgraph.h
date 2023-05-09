#ifndef EQGRAPH_H
#define EQGRAPH_H

#include <QGraphicsView>
#include <QList>
class EqDragPoint;
enum EqType{
    EQ_HIGHPASS = 0,
    EQ_LOWPASS,
    EQ_HIGHSHELF,
    EQ_LOWSHELF,
    EQ_NORMAL,
    EQ_SHG,
    EQ_BANDPASS_H,
    EQ_BANDPASS_L,
    EQ_NORMAL_CONFIG,
    EQ_SHG_CONFIG,
    EQ_HIGHPASS_CONFIG,
    EQ_LOWPASS_CONFIG,
    EQ_HIGHSHELF_CONFIG,
    EQ_LOWSHELF_CONFIG,
    EQ_BANDPASS_CONFIG,
    EQ_ALLPASS_CONFIG,
    EQ_CUSTOM_FIR_CONFIG,
};

struct EqPara
{
    int freq;
    int gain;
    int q;
    bool bypass;
    EqType type;
    float *result;
    EqDragPoint *dragPoint;
    EqPara(int fs, int gain, int q, EqType type)
    {
        this->freq = fs;
        this->gain = gain;
        this->q = q;
        this->type = type;
        this->bypass = false;
        this->result = NULL;
        this->dragPoint = NULL;
    }

    EqPara(int fs, int gain, int q)
    {
        this->freq = fs;
        this->gain = gain;
        this->q = q;
        this->bypass = false;
        this->type = EQ_NORMAL;
        this->result = NULL;
        this->dragPoint = NULL;
    }
};

class EqGraph : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(bool isBypass READ isBypass WRITE setBypass)
    Q_PROPERTY(bool isShowDrag READ isShowDrag WRITE setShowDrag)
    Q_PROPERTY(bool isShowCurve READ isShowCurve WRITE setShowCurve)
public:
    enum{
        GRID_ZVALUE = 0,        //边框标尺
        PATH_ZVALUE = 5,        //曲线
        DRAG_ZVALUE = 10,       //拖拽点
        ACTIVE_DRAG_ZVALUE = 20,//选中拖拽点
        TEXT_HIGH = 20,         //坐标文字高度
        X_LEN = 201,            //X轴取样长度
        SCALE_CNT = 201         //Eq参数取值范围
    };

public:
    EqGraph(QList<EqPara *> eqParaList, QWidget *parent = 0);
    EqGraph(QWidget *parent = 0);
    ~EqGraph();

    void initParaList(int freq, int gain, int q, EqType type);
    void initParaList(int freq);
    void initEqParaList(QList<EqPara *> eqParaList);
    void initParaListEnd(void);

    void setValue(int idx, int fs, int gain, int q);
    void setSegBypass(int idx, bool enable);
    void setSegInvert(int idx, bool enable);
    void setType(int idx, EqType type);
    void setCoeff(float *coeff, int len);
    void setCurrentDragPoint(EqDragPoint* dragPoint);
    void setCurrentDragPoint(int idx);
    EqDragPoint* getDragPoint(int idx);
    void eqParaChanged(int idx, int posX, int posY);

    bool isBypass() { return _isBypass;}
    bool isShowDrag() { return _isShowDrag;}
    bool isShowCurve() { return _isShowCurve;}
    void setPassType(int idx) { _passType = idx; calcEq(0);}
    EqPara* getEqPara(int idx) {return _eqParaList.at(idx);}

    void dispPointLabel(bool flag, int idx=0, QPointF pos=QPointF(0,0));
    void setPointLabelPos(int idx, QPointF pos);

signals:
    void valueChanged(int idx, int fx, int gain, int q, EqType type);
    void valueChanged(int idx, int fx, int gain, int q);
    void selectPointChanged(int idx);

public slots:
    //坐标转换 type 0=>fx, 1=>gain, 2= q
    int Float2Scale(float fx, int type);
    float Scale2Float(int scaleVal, int type);
    void restrictGain(float& value); //限制在-20~20之间
    void restrictScale(int& value); //限制在0~200之间

    void setBypass(bool isBypass);
    void setHPLPBypass(bool HPBypass, bool LPBypass);
    void resetGain();
    void onGainChanged(int idx, int gain);
    void setShowDrag(bool isShowDrag);
    void setShowCurve(bool isShowCurve);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void reflashPath();
    void calcEq(int idx);
    void calConstData();
    void initGraph();
    void initAxis();
    float getDistance(QPointF point1, QPointF point2);
    void setRestrictRect(int idx, EqDragPoint* dragPoint);

private:
    EqDragPoint* _curDragPoint;
    int _curIndex;
    QGraphicsScene* _scene;
    QGraphicsRectItem* _graphsItem; //边框标尺等
    QGraphicsPathItem* _pathCurves; //eq计算曲线
    QGraphicsPathItem* _pathArea; //eq计算区域
    QGraphicsPathItem* _pathCurrent; //eq计算区域
    QGraphicsTextItem* _pointText;
    QGraphicsTextItem* _bypassText;
    QList<float>  _fxAxisPoints;    //频率标尺点
    float m_x_tab[X_LEN], m_xFs_tab[X_LEN];
    QList<EqPara*> _eqParaList;
    float _customFirCoeff[1024];
    int _customFirCoeff_Len;
    bool _isBypass;
    bool _isShowDrag;
    bool _isShowCurve;
    int _width;  //图表宽度，预留最大半径
    int _height; //图表高度
    int _margin;
    float _scale_gain;
    float _scale_fs;

    /* EQ_BANDPASS 模式 */
    QGraphicsPathItem* _pathCurrentHP;
    QGraphicsPathItem* _pathCurrentLP;
    bool _isBypassHP;
    bool _isBypassLP;
    int _passType;
    int _scale_cnt_special;
    EqType _curType;
    bool _invert;
};
#endif //EQGRAPH_H
