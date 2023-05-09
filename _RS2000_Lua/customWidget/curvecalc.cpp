#include "curvecalc.h"
#include <QtCore/qmath.h>
#include "eqgraph.h"

void CurveCalc::calc(EqType type, float fs, float gain, float Q, int invert,
    float* fx, int fx_len,
    float *coeff, int coeff_len,
    float* rs, int passType)
{
//    qDebug() << __FUNCTION__ << __LINE__ << Q;
    switch(type) {
    case EQ_HIGHPASS:
    case EQ_HIGHPASS_CONFIG:
//        hpCalc(fs, len, fx, rs);
        if (passType > 3)
        {
            bwHpCalc(fs, passType - 3, fx_len, fx, rs);
        }
        else
        {
            lrCalc(fs, passType + 1, fx_len, fx, rs, true);
        }
        break;
    case EQ_LOWPASS:
    case EQ_LOWPASS_CONFIG:
//        lpCalc(fs, len, fx, rs);
        if (passType > 3)
        {
            bwLpCalc(fs, passType - 3, fx_len, fx, rs);
        }
        else
        {
            lrCalc(fs, passType + 1, fx_len, fx, rs, false);
        }
        break;
    case EQ_HIGHSHELF:
    case EQ_HIGHSHELF_CONFIG:
        sheftCalc(fs, gain, fx_len, fx, rs, true);
        break;
    case EQ_LOWSHELF:
    case EQ_LOWSHELF_CONFIG:
        sheftCalc(fs, gain, fx_len,  fx, rs, false);
        break;
    case EQ_NORMAL:
    case EQ_SHG:
    case EQ_NORMAL_CONFIG:
        eqCalc(fs, gain, Q, fx_len, fx, rs);
        break;
    case EQ_SHG_CONFIG:
        eqSpecialCalc(fs, gain, Q, fx_len, fx, rs);
        break;
    case EQ_BANDPASS_H:
        hpCalc(fs, fx_len, fx, rs);
        break;
    case EQ_BANDPASS_L:
        lpCalc(fs, fx_len, fx, rs);
        break;
    case EQ_BANDPASS_CONFIG:
        bwBandpassCalc(fs, Q, 2, fx_len, fx, rs);
        break;
    case EQ_ALLPASS_CONFIG:
        allPassCalc(fs, Q, invert, fx, fx_len, rs);
        break;
    case EQ_CUSTOM_FIR_CONFIG:
        customFirCalc(coeff, coeff_len, fx, fx_len, rs);
    default:
        break;
    }
}

void CurveCalc::fc_allpass_secondiir_coffeProduce(float PI_Phase_fTo1,float gain_db,float fQ,unsigned int antiphase,float a[3],float b[3])
{
    PI_Phase_fTo1 = fabsf(PI_Phase_fTo1);
    if(PI_Phase_fTo1 > 1.0f)	PI_Phase_fTo1 = 1.0f;
    float w0 = PI_Phase_fTo1 * PI;
    float sg = powf(10.0f, gain_db*0.05);
    if(antiphase)	sg*=-1.0f;
    float alpha = sinf(w0) / (2 * fabsf(fQ));
    b[0] = (1 - alpha) * sg;
    b[1] = -2 * cosf(w0) * sg;
    b[2] = (1 + alpha) * sg;
    a[0] = 1 + alpha;
    a[1] = -2 * cosf(w0);
    a[2] = 1 - alpha;
    float shrink = 1.0f/a[0];
    for(int i = 0;i<3;i++){
        a[i]*=shrink;
        b[i]*=shrink;
    }
}

void CurveCalc::fc_pfirTool_getFreqAmpPhase(float getfTo1,float * coeff,float *p_gain_times,float *p_gain_db,float *p_phase,unsigned int firCoeffLen)
{
    getfTo1 = fabsf(getfTo1);
    if(coeff == NULL || getfTo1 > 1.0f || firCoeffLen == 0)	return;
    float pi = 3.14159265;
    float cost = cosf(getfTo1*pi);
    float sint = sinf(getfTo1*pi);
    float rel,ima;
    if(firCoeffLen == 1){
        rel = coeff[0];
        ima = 0.0f;
    }
    else{
        float coso = cost,sino = sint;
        float cosn,sinn;
        rel = coeff[0] + coeff[1]*cost;
        ima = -coeff[1]*sint;
        for(int i = 2;i<firCoeffLen;i++){
            cosn = cost*coso - sint*sino;
            sinn = sint*coso + cost*sino;
            rel += coeff[i]*cosn;
            ima -= coeff[i]*sinn;
            coso = cosn;
            sino = sinn;
//            qDebug() << __FUNCTION__ << __LINE__ << i << coeff[i] << cosn << sinn << rel << ima << coso << sino;
        }
    }
//    qDebug() << __FUNCTION__ << __LINE__ << getfTo1 << coeff[0] << firCoeffLen << rel << ima << rel*rel + ima*ima + 1e-30;
    if(p_gain_times)	*p_gain_times = sqrtf(rel*rel + ima*ima + 1e-15);
    if(p_gain_db)		*p_gain_db = 10.0f*log10f(rel*rel + ima*ima + 1e-30);
    if(p_phase){
        float tt = sqrtf(rel*rel + ima*ima);
        if(tt == 0){
            *p_phase = 0;
        }
        else{
            if(ima >= 0)	*p_phase = acos(rel/tt);
            else			*p_phase = -acos(rel/tt);
        }
    }
}

void CurveCalc::fc_butterworthm_bpass2cx_coffeProduce(float floorfTo1,float upperfTo1,float a[3],float b[3])
{
    memset(a,0,sizeof(float)*3);
    memset(b,0,sizeof(float)*3);
    a[0] = 1.0f;
    floorfTo1 = fabsf(floorfTo1);
    upperfTo1 = fabsf(upperfTo1);
    if(upperfTo1 > 0.999f)	upperfTo1 = 0.999f;
    if(floorfTo1 < 0.001f)	floorfTo1 = 0.001f;
    if(floorfTo1 >= upperfTo1)	{
        b[0] = 1.0f;
        return;
    }
    float q;
    float coswsm = sinf((floorfTo1 + upperfTo1)*PI)/(sinf(upperfTo1*PI)+sinf(floorfTo1*PI));		//帯通滤波器中心频率的余弦值
    float alf_wac = fabsf((coswsm - cosf(floorfTo1*PI))/sinf(floorfTo1*PI));						//帯通滤波器双线性变换需要的匹配的低通滤波器的截止频率
    q = 1.0f;
    b[0] = alf_wac;
    b[1] = 0;
    b[2] = -alf_wac;
    a[0] = 1.0f + q*alf_wac;
    a[1] = -2.0f*coswsm;
    a[2] = 1.0f - q*alf_wac;
    return;
}

bool CurveCalc::fc_iir_getFreqFeature(unsigned int iirOrdrs,float a[],float b[],float getfTo1,float *p_gain_times,float *p_gain_db,float *p_phase)
{
    if(iirOrdrs == 0 || getfTo1 < 0.0f || getfTo1 > 1.0f)
        return false;
    float kr,ki,hr,hi;
    float gain = 1e-15;
    kr = b[iirOrdrs];
    ki = 0;
    hr = a[iirOrdrs];
    hi = 0;
    for(int i = 0;i<iirOrdrs;i++){
        unsigned int m = iirOrdrs -i;
        kr += b[i]*cosf(m*getfTo1*PI);
        ki += b[i]*sinf(m*getfTo1*PI);
        hr += a[i]*cosf(m*getfTo1*PI);
        hi += a[i]*sinf(m*getfTo1*PI);
    }
    if(p_gain_times||p_gain_db){
        float k = kr*kr+ki*ki;
        float h = hr*hr+hi*hi;
        if(h == 0){
            gain = 1e15;
        }
        else{
            gain = sqrtf(k/h);
            if(gain < 1e-15){
                gain = 1e-15;
            }
            if(gain > 1e15){
                gain = 1e15;
            }
        }
        if(p_gain_times)
            *p_gain_times = gain;
        if(p_gain_db)
            *p_gain_db = 20.0f*log10f(gain);
    }
    if(p_phase){
        float pr = kr*hr + ki*hi;
        float pi = ki*hr - kr*hi;
        float ps = sqrtf(pr*pr+pi*pi);
        if(ps == 0.0f){
            *p_phase = 0.0f;
        }
        else{
            float phase = acosf(pr/ps);
            if(pi>=0.0f)	*p_phase = phase;
            else	*p_phase = -phase;
        }
    }
    return true;
}

void CurveCalc::bwBandpassCalc(float fs, float Q, int n, int len, float *fx, float *rs)
{
    float low_freq = fs / qPow(2, Q);
    float top_freq = fs * qPow(2, Q);
    float a[3], b[3];
    fc_butterworthm_bpass2cx_coffeProduce(low_freq / 24000.0f, top_freq / 24000.0f, a, b);

    for (int i = 0; i < len; ++i) {
        fc_iir_getFreqFeature(n, a, b, fx[i] / 24000.0f, NULL, &rs[i], NULL);
    }
}

void CurveCalc::allPassCalc(float freq, float q, unsigned int phase, float *fx, int fx_len, float *rs)
{
    float a[3], b[3];
    fc_allpass_secondiir_coffeProduce(freq / 24000.0f, 0, q, phase, a, b);
//    qDebug() << __FUNCTION__ << __LINE__ << freq << q << phase << a[0] << a[1] << a[2] << b[0] << b[1] << b[2];
    for (int i = 0; i < fx_len; ++i) {
        fc_iir_getFreqFeature(2, a, b, fx[i] / 24000.0f, NULL, NULL, &rs[i]);
//        qDebug() << __FUNCTION__ << __LINE__ << i << fx[i] << rs[i];
    }
}

void CurveCalc::customFirCalc(float *coeff, int coeff_len, float *fx, int fx_len, float *rs)
{
    if (coeff[0] == 0 && coeff[1] == 0 && coeff[2] == 0 && coeff[3] == 0 && coeff[4] == 0
            && coeff[5] == 0 && coeff[6] == 0 && coeff[7] == 0 && coeff[8] == 0 && coeff[9] == 0) {
        for (int i = 0; i < fx_len; ++i) {
            rs[i] = 0;
        }
    }
    else {
        for (int i = 0; i < fx_len; ++i) {
            fc_pfirTool_getFreqAmpPhase(fx[i] / 24000.0f, coeff, NULL, &rs[i], NULL, coeff_len);
        }
    }
//    qDebug() << __FUNCTION__ << __LINE__ << coeff_len << rs[0];
}

void CurveCalc::bwHpCalc(float f, int n, int len, float *fx, float *rs)
{
    float k = 3 - (1.0f / 0.707f);
    float w0 = 2 * 3.1416 * f;
    for(int i = 0; i < len; i++)
    {
        float w = 2 * 3.1416 * fx[i];
        float y0 = k * k * w * w * w * w;
        float y1 = (w0 * w0 - w * w);
        y1 = y1 * y1;
        float temp = (w * w0 / 0.707);
        temp = temp * temp;
        y1 = y1 + temp;
        rs[i] = n * (10 * (log(y0) - log(y1)) / log(10) - 4.0);
    }
}

void CurveCalc::bwLpCalc(float f, int n ,int len, float *fx, float *rs)
{
    float k = 1.5856f;
    float w0 = 2 * 3.1416 * f;
    for(int i = 0; i < len; i++)
    {
        float w = 2 * 3.1416 * fx[i];
        float y0 = k * k * w0 * w0 * w0 * w0;
        float y1 = (w0 * w0 - w * w);
        y1 = y1 * y1;
        float y2 = (w * w0 / 0.707);
        y2 = y2 * y2;
        y1 = y1 + y2;
        rs[i] = n * (10 * (log(y0) - log(y1)) / log(10) - 4.0);
    }
}

void CurveCalc::lrCalc(float f, int n, int len, float *fx, float *rs, bool isHigh)
{
//    float W0, A;
    float b0, b1, b2, a0, a1, a2;
    float wc,wc2,wc22,k,k2,k22,wck2,tmpk;
    float num[3];
    float den[3];

//    A = 1.0;
//    W0 = 2 * M_PI * f/ 48000;

    wc=2*M_PI*f;
    wc2=wc*wc;
    wc22=2*wc2;
    k=wc/tan(M_PI*f/48000.0);//正切函数
    k2=k*k;
    k22=2*k2;
    wck2=2*wc*k;
    tmpk=(k2+wc2+wck2);

    if (isHigh)
    {
        a0=1.0;
        a1 = (-k22+wc22)/tmpk;
        a2 = (-wck2+k2+wc2)/tmpk;
        b0 = (k2)/tmpk;
        b1 = (-k22)/tmpk;
        b2 = (k2)/tmpk;
    }
    else
    {
        a0=1.0;
        a1 = (-k22+wc22)/tmpk;
        a2 = (-wck2+k2+wc2)/tmpk;
        b0 = (wc2)/tmpk;
        b1 = (wc22)/tmpk;
        b2 = (wc2)/tmpk;
    }

    num[0]=(float)(b0/a0);
    num[1]=(float)(b1/a0);
    num[2]=(float)(b2/a0);
    den[0] = (float)1.0;
    den[1]=(float)(a1/a0);
    den[2]=(float)(a2/a0);

    //计算gain
    float zr, zi;
    float re, im;
    float ar, ai, br, bi;
    float numr, numi;
    float de, temp;
    for(int k = 0; k < len; k++)
    {
        float freq = fx[k]/48000;
        zr = cos(-2.0 * M_PI * freq);
        zi = sin(-2.0 * M_PI * freq);
        br = 0.0;
        bi = 0.0;
        for(int i = SHELF_ORDER; i > 0; i--)
        {
            re = br;
            im = bi;
            br = (re + num[i]) * zr - im * zi;
            bi = (re + num[i]) * zi + im * zr;
        }
        ar = 0.0;
        ai = 0.0;
        for(int i = SHELF_ORDER; i > 0; i--)
        {
            re = ar;
            im = ai;
            ar = (re + den[i]) * zr - im * zi;
            ai = (re + den[i]) * zi + im * zr;
        }

        br = br + num[0];
        ar = ar + 1.0;
        numr = ar * br + ai * bi;
        numi = ar * bi - ai * br;
        de = ar * ar + ai * ai;

        temp = (numr * numr + numi * numi)/(de * de);
        rs[k] = n * 10 *log10(temp);
    }
}

void CurveCalc::hpCalc(float f, int len, float *fx, float *rs)
{
    float k = 3 - (1.0f / 0.707f);
    float w0 = 2 * 3.1416 * f;
    for(int i = 0; i < len; i++)
    {
        float w = 2 * 3.1416 * fx[i];
        float y0 = k * k * w * w * w * w;
        float y1 = (w0 * w0 - w * w);
        y1 = y1 * y1;
        float temp = (w * w0 / 0.707);
        temp = temp * temp;
        y1 = y1 + temp;
        rs[i] = 10 * (log(y0) - log(y1)) / log(10) - 4.0;
    }
}

void CurveCalc::lpCalc(float f, int len, float *fx, float *rs)
{
    float k = 1.5856f;
    float w0 = 2 * 3.1416 * f;
    for(int i = 0; i < len; i++)
    {
        float w = 2 * 3.1416 * fx[i];
        float y0 = k * k * w0 * w0 * w0 * w0;
        float y1 = (w0 * w0 - w * w);
        y1 = y1 * y1;
        float y2 = (w * w0 / 0.707);
        y2 = y2 * y2;
        y1 = y1 + y2;
        rs[i] = 10 * (log(y0) - log(y1)) / log(10) - 4.0;
    }
}

void CurveCalc::eqCalc(float f, float gain, float q, int len, float *fx, float *rs)
{
    float z1, z2, z3, z4, zq;
//    if () {
//        zq = 1/q;
//    }else {
//        zq = q;
//    }
    float Q1 = (float)((powf(2.0,0.5*1/q))/(powf(2.0,1/q)-1.0)) * 1.4f;

    for(int i = 0; i < len; i++)
    {
        z1 = fx[i] / f;
        z2 = z1 - (1 / z1);
        z3 = 1 + Q1 * Q1 * z2 * z2;
        z4 = sqrt(z3);
        rs[i] = gain / z4;
    }
}

void CurveCalc::eqSpecialCalc(float f, float gain, float q, int len, float *fx, float *rs)
{
    float z1, z2, z3, z4;
    float Q1 = q * 2.9f;

    for(int i = 0; i < len; i++)
    {
        z1 = fx[i] / f;
        z2 = z1 - (1 / z1);
        z3 = 1 + Q1 * Q1 * z2 * z2;
        z4 = sqrt(z3);
        rs[i] = gain / z4;
    }
}

void CurveCalc::sheftCalc(float f, float gain, int len,  float *fx, float *rs, bool isHigh)
{
    //计算参数
    float q = 0.468f;
    float num[3];
    float den[3];
    float w0=(float)2*M_PI*f/48000;
    float cosV=cosf(w0);
    float ALP = sinf(w0) / (2 * q);
    float a0,a1,a2,b0,b1,b2;
    float A=powf(10.0,gain/40.0);
    if (isHigh)
    {
        a0 = (A + 1) - (A - 1)*cosV + 2 * sqrt(A)*ALP;
        a1 = 2 * ((A - 1) - (A + 1)*cosV);
        a2 = (A + 1) - (A - 1)*cosV - 2 * sqrt(A)*ALP;
        b0 = A*((A + 1) + (A - 1)*cosV + 2 * sqrt(A)*ALP);
        b1 = -2 * A*((A - 1) + (A + 1)*cosV);
        b2 = A*((A + 1) + (A - 1)*cosV - 2 * sqrt(A)*ALP);
    }
    else
    {
        a0 = (A + 1) + (A - 1)*cosV + 2 * sqrt(A)*ALP;
        a1 = -2 * ((A - 1) + (A + 1)*cosV);
        a2 = (A + 1) + (A - 1)*cosV - 2 * sqrt(A)*ALP;
        b0 = A*((A + 1) - (A - 1)*cosV + 2 * sqrt(A)*ALP);
        b1 = 2 * A*((A - 1) - (A + 1)*cosV);
        b2 = A*((A + 1) - (A - 1)*cosV - 2 * sqrt(A)*ALP);
    }
    num[0]=(float)(b0/a0);
    num[1]=(float)(b1/a0);
    num[2]=(float)(b2/a0);
    den[0] = (float)1.0;
    den[1]=(float)(a1/a0);
    den[2]=(float)(a2/a0);
    //计算gain
    float zr, zi;
    float re, im;
    float ar, ai, br, bi;
    float numr, numi;
    float de, temp;
    for(int k = 0; k < len; k++)
    {
        float freq = fx[k]/48000;
        zr = cos(-2.0 * M_PI * freq);
        zi = sin(-2.0 * M_PI * freq);
        br = 0.0;
        bi = 0.0;
        for(int i = SHELF_ORDER; i > 0; i--)
        {
            re = br;
            im = bi;
            br = (re + num[i]) * zr - im * zi;
            bi = (re + num[i]) * zi + im * zr;
        }
        ar = 0.0;
        ai = 0.0;
        for(int i = SHELF_ORDER; i > 0; i--)
        {
            re = ar;
            im = ai;
            ar = (re + den[i]) * zr - im * zi;
            ai = (re + den[i]) * zi + im * zr;
        }

        br = br + num[0];
        ar = ar + 1.0;
        numr = ar * br + ai * bi;
        numi = ar * bi - ai * br;
        de = ar * ar + ai * ai;

        temp = (numr * numr + numi * numi)/(de * de);
        rs[k] = 10 *log10(temp);
    }
}
