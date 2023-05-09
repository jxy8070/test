#ifndef CURVECALC_H
#define CURVECALC_H

enum EqType;
#define PI 3.14159265358

class CurveCalc
{
    enum {SHELF_ORDER =2};
public:
    static void calc(EqType type, float fs, float gain, float Q, int invert, float* fx, int fx_len, float *coeff, int coeff_len, float *rs, int passType);
    static void bwHpCalc(float f, int n, int len, float *fx, float *rs);
    static void bwLpCalc(float f, int n, int len, float *fx, float *rs);
    static void lrCalc(float f, int n, int len, float *fx, float *rs, bool isHigh);
    static void bwBandpassCalc(float fs, float Q, int n, int len, float *fx, float *rs);
    static void allPassCalc(float freq, float q, unsigned int phase, float *fx, int fx_len, float *rs);
    static void customFirCalc(float *coeff, int coeff_len, float *fx, int fx_len, float *rs);
    static void fc_butterworthm_bpass2cx_coffeProduce(float floorfTo1,float upperfTo1,float a[3],float b[3]);
    static bool fc_iir_getFreqFeature(unsigned int iirOrdrs,float a[],float b[],float getfTo1,float *p_gain_times,float *p_gain_db,float *p_phase);
    static void fc_pfirTool_getFreqAmpPhase(float getfTo1,float * coeff,float *p_gain_times,float *p_gain_db,float *p_phase,unsigned int firCoeffLen);
    static void fc_allpass_secondiir_coffeProduce(float PI_Phase_fTo1,float gain_db,float fQ,unsigned int antiphase,float a[3],float b[3]);
private:
    static void eqCalc(float f, float gain, float q, int len, float *fx, float *rs);
    static void eqSpecialCalc(float f, float gain, float q, int len, float *fx, float *rs);
    static void sheftCalc(float f, float gain, int len, float *fx, float *rs, bool isHigh);
    static void hpCalc(float f, int len, float *fx, float *rs);
    static void lpCalc(float f, int len, float *fx, float *rs);
};

#endif // CURVECALC_H
