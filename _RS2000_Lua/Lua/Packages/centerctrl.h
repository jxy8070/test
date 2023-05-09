#ifndef CENTERCTRL_H
#define CENTERCTRL_H

#include <QNetworkDatagram>


struct UdpGram{
    friend QDataStream& operator<<(QDataStream& stream, const UdpGram& gram);
    friend QDataStream& operator>>(QDataStream& stream, UdpGram& gram);
    UdpGram(const QString itemName, quint32 cmdKey);
    UdpGram();
    quint32 getCmdKey();
    void setUint16(quint16 value, quint8 index);
    quint16 getUint16(quint8 index);
    void setGain(double gain, quint8 index);
    double getGain(quint8 index);
    inline void setBool(bool state, quint8 index){ data[index] = (state? 0x01 : 0x00); }
    inline bool getBool(quint8 index) { return data[index] == 0x01? true : false; }
    inline void setUint8(quint8 chNo, quint8 index) { data[index] = chNo; }
    inline quint8 getUint8(quint8 index) { return (quint8)data[index]; }

private:
    char name[8];               //模块名称,只取前8字节,不足补0x00
    quint32 cmdKey;             //命令传递
    unsigned char data[8];      //命令参数传递
};


class CenterCtrl : public QObject
{
    Q_OBJECT

public:
    void setDevAddress(QString devIp);
    bool getCurSceneNo(int& sceneNo);

    //================输入模块================//
    bool getInputAntiPhase(const QString& itemName, quint8 chNo, bool& antiPhase);      //0x00000001
    bool setInputAntiPhase(const QString& itemName, quint8 chNo, bool antiPhase);       //0x00000002
    bool getInputMute(const QString& itemName, quint8 chNo, bool& isMute);              //0x00000003
    bool setInputMute(const QString& itemName, quint8 chNo, bool isMute);               //0x00000004
    bool getInputGain(const QString& itemName, quint8 chNo, double& gain);              //0x00000005
    bool setInputGain(const QString& itemName, quint8 chNo, double gain);               //0x00000006

    //================输出模块================//
    bool getOutputAntiPhase(const QString& itemName, quint8 chNo, bool& antiPhase);     //0x00010001
    bool setOutputAntiPhase(const QString& itemName, quint8 chNo, bool antiPhase);      //0x00010002
    bool getOutputMute(const QString& itemName, quint8 chNo, bool& isMute);             //0x00010003
    bool setOutputMute(const QString& itemName, quint8 chNo, bool isMute);              //0x00010004
    bool getOutputGain(const QString& itemName, quint8 chNo, double& gain);             //0x00010005
    bool setOutputGain(const QString& itemName, quint8 chNo, double gain);              //0x00010006

    //================延时模块================//
    bool getDelayBypass(const QString& itemName, bool& bypass);                         //0x00020001
    bool setDelayBypass(const QString& itemName, bool bypass);                          //0x00020002
    bool getDelayTime(const QString& itemName, double& ms, double& maxMs);              //0x00020003
    bool setDelayTime(const QString& itemName, double ms);                              //0x00020004

    //================路由模块================//
    bool getRouterOutput(const QString& itemName, quint8 chNo, quint8& inCh);           //0x00030001
    bool setRouterOutput(const QString& itemName, quint8 chNo, quint8 inCh);            //0x00030002

    //================混音模块================//
    bool getMixerInputState(const QString& itemName, quint8 chNo, bool& state);         //0x00040001
    bool setMixerInputState(const QString& itemName, quint8 chNo, bool state);          //0x00040002
    bool getMixerInputGain(const QString& itemName, quint8 chNo, double& gain);         //0x00040003
    bool setMixerInputGain(const QString& itemName, quint8 chNo, double gain);          //0x00040004
    bool getMixerOutputState(const QString& itemName, quint8 chNo, bool& state);        //0x00040005
    bool setMixerOutputState(const QString& itemName, quint8 chNo, bool state);         //0x00040006
    bool getMixerOutputGain(const QString& itemName, quint8 chNo, double& gain);        //0x00040007
    bool setMixerOutputGain(const QString& itemName, quint8 chNo, double gain);         //0x00040008
    bool getMixerNodeState(const QString& itemName, quint8 chIn, quint8 chOut, bool& state);        //0x00040009
    bool setMixerNodeState(const QString& itemName, quint8 chIn, quint8 chOut, bool state);         //0x0004000A
    bool getMixerNodeGain(const QString& itemName, quint8 chIn, quint8 chOut, double& gain);        //0x0004000B
    bool setMixerNodeGain(const QString& itemName, quint8 chIn, quint8 chOut, double gain);         //0x0004000C

    //================动态模块================//
    bool getDynThreshold(const QString& itemName, double& gain);                        //0x00050001
    bool setDynThreshold(const QString& itemName, double gain);                         //0x00050002
    bool getDynRatio(const QString& itemName, double& ratio);                           //0x00050003
    bool setDynRatio(const QString& itemName, double ratio);                            //0x00050004
    bool getDynAttackTime(const QString& itemName, quint16& ms, quint16& maxMs);        //0x00050005
    bool setDynAttackTime(const QString& itemName, quint16 ms);                         //0x00050006
    bool getDynReleaseTime(const QString& itemName, quint16& ms, quint16& maxMs);       //0x00050007
    bool setDynReleaseTime(const QString& itemName, quint16 ms);                        //0x00050008
    bool getDynGain(const QString& itemName, double& gain);                             //0x00050009
    bool setDynGain(const QString& itemName, double gain);                              //0x0005000A
    bool getDynTargetLevel(const QString& itemName, double& gain);                      //0x0005000B
    bool setDynTargetLevel(const QString& itemName, double gain);                       //0x0005000C
    bool getDynBypass(const QString& itemName, bool& bypass);                           //0x0005000D
    bool setDynBypass(const QString& itemName, bool bypass);                            //0x0005000E

    //================均衡模块================//
    bool getEqFreq(const QString& itemName, quint8 ptIdx, quint16& freq);               //0x00060001
    bool setEqFreq(const QString& itemName, quint8 ptIdx, quint16 freq);                //0x00060002
    bool getEqGain(const QString& itemName, quint8 ptIdx, double& gain, double& maxGain, double& minGain);  //0x00060003
    bool setEqGain(const QString& itemName, quint8 ptIdx, double gain);                 //0x00060004
    bool getEqQVal(const QString& itemName, quint8 ptIdx, double& qVal);                //0x00060005
    bool setEqQVal(const QString& itemName, quint8 ptIdx, double qVal);                 //0x00060006
    bool getEqBypass(const QString& itemName, bool& bypass);                            //0x00060007
    bool setEqBypass(const QString& itemName, bool bypass);                             //0x00060008

    //================信号发生器模块================//
    bool getSigFreq(const QString& itemName, quint16& freq);                            //0x00070001
    bool setSigFreq(const QString& itemName, quint16 freq);                             //0x00070002
    bool getSigGain(const QString& itemName, double& gain);                             //0x00070003
    bool setSigGain(const QString& itemName, double gain);                              //0x00070004
    bool getSigMute(const QString& itemName, bool& mute);                               //0x00070005
    bool setSigMute(const QString& itemName, bool mute);                                //0x00070006

    //================分频器模块================//
    bool getSplitInMute(const QString& itemName, bool& mute);                           //0x00080001
    bool setSplitInMute(const QString& itemName, bool mute);                            //0x00080002
    bool getSplitInGain(const QString& itemName, double& gain);                         //0x00080003
    bool setSplitInGain(const QString& itemName, double gain);                          //0x00080004
    bool getSplitType(const QString& itemName, quint8 sNo, quint8& type);               //0x00080005
    bool setSplitType(const QString& itemName, quint8 sNo, quint8 type);                //0x00080006
    bool getSplitFreq(const QString& itemName, quint8 sNo, quint16& freq);              //0x00080007
    bool setSplitFreq(const QString& itemName, quint8 sNo, quint16 freq);               //0x00080008
    bool getSplitOutMute(const QString& itemName, bool& mute);                          //0x00080009
    bool setSplitOutMute(const QString& itemName, bool mute);                           //0x0008000A
    bool getSplitOutGain(const QString& itemName, double& gain);                        //0x0008000B
    bool setSplitOutGain(const QString& itemName, double gain);                         //0x0008000C
    bool getSplitOutAntiPhase(const QString& itemName, bool& antiPhase);                //0x0008000D
    bool setSplitOutAntiPhase(const QString& itemName, bool antiPhase);                 //0x0008000E

    //================滤波器模块================//
    bool getFilterType(const QString& itemName, quint8& type);                          //0x00090001
    bool setFilterType(const QString& itemName, quint8 type);                           //0x00090002
    bool getFilterFreq(const QString& itemName, quint16& freq);                         //0x00090003
    bool setFilterFreq(const QString& itemName, quint16 freq);                          //0x00090004
    bool getFilterGain(const QString& itemName, double& gain);                          //0x00090005
    bool setFilterGain(const QString& itemName, double gain);                           //0x00090006
    bool getFilterBypass(const QString& itemName, bool& bypass);                        //0x00090007
    bool setFilterBypass(const QString& itemName, bool bypass);                         //0x00090008
    bool getFilterBandwidth(const QString& itemName, double& bandWidth);                //0x00090009
    bool setFilterBandwidth(const QString& itemName, double bandWidth);                 //0x0009000A

    //================啸叫抑制模块================//
    bool getFreqshiftFreq(const QString& itemName, quint16& freq);                      //0x000A0001
    bool setFreqshiftFreq(const QString& itemName, quint16 freq);                       //0x000A0002
    bool getFreqshiftGain(const QString& itemName, double& gain);                       //0x000A0003
    bool setFreqshiftGain(const QString& itemName, double gain);                        //0x000A0004

    //================自动混音模块================//
    bool getAutoMixerThreshold(const QString& itemName, double& gain);                  //0x000B0001
    bool setAutoMixerThreshold(const QString& itemName, double gain);                   //0x000B0002
    bool getAutoMixerAttackTime(const QString& itemName, quint16& ms, quint16& maxMs);   //0x000B0003
    bool setAutoMixerAttackTime(const QString& itemName, quint16 ms);                   //0x000B0004
    bool getAutoMixerReleaseTime(const QString& itemName, quint16& ms, quint16& maxMs);  //0x000B0005
    bool setAutoMixerReleaseTime(const QString& itemName, quint16 ms);                  //0x000B0006
    bool getAutoMixerHoldTime(const QString& itemName, quint16& ms, quint16& maxMs);     //0x000B0007
    bool setAutoMixerHoldTime(const QString& itemName, quint16 ms);                     //0x000B0008
    bool getAutoMixerDepth(const QString& itemName, double& gain);                      //0x000B000B
    bool setAutoMixerDepth(const QString& itemName, double gain);                       //0x000B000C
    bool getAutoMixerNoiseGain(const QString& itemName, double& gain);                  //0x000B000F
    bool setAutoMixerNoiseGain(const QString& itemName, double gain);                   //0x000B0010
    bool getAutoMixerInputMute(const QString& itemName, quint8 chNo, bool& mute);       //0x000B0011
    bool setAutoMixerInputMute(const QString& itemName, quint8 chNo, bool mute);        //0x000B0012
    bool getAutoMixerInputGain(const QString& itemName, quint8 chNo, double& gain);     //0x000B0013
    bool setAutoMixerInputGain(const QString& itemName, quint8 chNo, double gain);      //0x000B0014
    bool getAutoMixerOutputMute(const QString& itemName, quint8 chNo, bool& mute);      //0x000B0015
    bool setAutoMixerOutputMute(const QString& itemName, quint8 chNo, bool mute);       //0x000B0016
    bool getAutoMixerOutputGain(const QString& itemName, quint8 chNo, double& gain);    //0x000B0017
    bool setAutoMixerOutputGain(const QString& itemName, quint8 chNo, double gain);     //0x000B0018
    bool getAutoMixerInputManual(const QString& itemName, quint8 chNo, bool& manual);   //0x000B0019
    bool setAutoMixerInputManual(const QString& itemName, quint8 chNo, bool manual);    //0x000B001A
    bool getAutoMixerInputMode(const QString& itemName, quint8 chNo, quint8& mode);     //0x000B001B
    bool setAutoMixerInputMode(const QString& itemName, quint8 chNo, quint8 mode);      //0x000B001C
    bool getAutoMixerInputPriority(const QString& itemName, quint8 chNo, quint16& prio);//0x000B001D
    bool setAutoMixerInputPriority(const QString& itemName, quint8 chNo, quint16 prio); //0x000B001E
    bool getAutoMixerPercentage(const QString& itemName, double& percent);              //0x000B001F
    bool setAutoMixerPercentage(const QString& itemName, double percent);               //0x000B0020
    bool getAutoMixerLastMicOn(const QString& itemName, quint8& status);                //0x000B0023
    bool setAutoMixerLastMicOn(const QString& itemName, quint8 status);                 //0x000B0024
    bool getAutoMixerDefaultCh(const QString& itemName, quint8& defCn);                 //0x000B0025
    bool setAutoMixerDefaultCh(const QString& itemName, quint8 defCn);                  //0x000B0026
    bool getAutoMixerThresholdLevelAboveNoise(const QString& itemName, double& gain);   //0x000B0027
    bool setAutoMixerThresholdLevelAboveNoise(const QString& itemName, double gain);    //0x000B0028
    bool getAutoMixerMaxNOM(const QString& itemName, bool& onOff);                      //0x000B0029
    bool setAutoMixerMaxNOM(const QString& itemName, bool onOff);                       //0x000B002A
    bool getAutoMixerDirectOutsNOMAtten(const QString& itemName, bool& onOff);          //0x000B002B
    bool setAutoMixerDirectOutsNOMAtten(const QString& itemName, bool onOff);           //0x000B002C
    bool getAutoMixerNOMAttenuationType(const QString& itemName, quint8& type);         //0x000B002D
    bool setAutoMixerNOMAttenuationType(const QString& itemName, quint8 type);          //0x000B002E
    bool getAutoMixerNOMAttenuationStep(const QString& itemName, double& gain);         //0x000B002F
    bool setAutoMixerNOMAttenuationStep(const QString& itemName, double gain);          //0x000B0030
    bool getAutoMixerMaxNOMAttenuation(const QString& itemName, double& gain);          //0x000B0031
    bool setAutoMixerMaxNOMAttenuation(const QString& itemName, double gain);           //0x000B0032
    bool getAutoMixerResponseTime(const QString& itemName, quint16& ms);                //0x000B0033
    bool setAutoMixerResponseTime(const QString& itemName, quint16 ms);                 //0x000B0034


    //================回声消除模块================//
    bool getAECMicInGain(const QString& itemName, double& gain);                        //0x000C0001
    bool setAECMicInGain(const QString& itemName, double gain);                         //0x000C0002
    bool getAECLineInGain(const QString& itemName, double& gain);                       //0x000C0003
    bool setAECLineInGain(const QString& itemName, double gain);                        //0x000C0004
    bool getAECSpeakOutGain(const QString& itemName, double& gain);                     //0x000C0005
    bool setAECSpeakOutGain(const QString& itemName, double gain);                      //0x000C0006
    bool getAECLineOutGain(const QString& itemName, double& gain);                      //0x000C0007
    bool setAECLineOutGain(const QString& itemName, double gain);                       //0x000C0008
    bool getAECMicToSpeakGain(const QString& itemName, double& gain);                   //0x000C0009
    bool setAECMicToSpeakGain(const QString& itemName, double gain);                    //0x000C000A
    bool getAECMicToSpeakGainOn(const QString& itemName, bool& onOff);                  //0x000C000B
    bool setAECMicToSpeakGainOn(const QString& itemName, bool onOff);                   //0x000C000C
    bool getAECNRGain(const QString& itemName, quint8& gainType);                       //0x000C000D
    bool setAECNRGain(const QString& itemName, quint8 gainType);                        //0x000C000E
    bool getAECNRGainOn(const QString& itemName, bool& onOff);                          //0x000C000F
    bool setAECNRGainOn(const QString& itemName, bool onOff);                           //0x000C0010

    //================闪避模块================//
    bool getDuckerAttackTime(const QString& itemName, quint16& ms);                     //0x000D0001
    bool setDuckerAttackTime(const QString& itemName, quint16 ms);                      //0x000D0002
    bool getDuckerReleaseTime(const QString& itemName, quint16& ms);                    //0x000D0003
    bool setDuckerReleaseTime(const QString& itemName, quint16 ms);                     //0x000D0004
    bool getDuckerHoldTime(const QString& itemName, quint16& ms);                       //0x000D0005
    bool setDuckerHoldTime(const QString& itemName, quint16 ms);                        //0x000D0006
    bool getDuckerDetectTime(const QString& itemName, quint16& ms);                     //0x000D0007
    bool setDuckerDetectTime(const QString& itemName, quint16 ms);                      //0x000D0008
    bool getDuckerThreshold(const QString& itemName, double& gain);                     //0x000D0009
    bool setDuckerThreshold(const QString& itemName, double gain);                      //0x000D000A
    bool getDuckerDepth(const QString& itemName, double& gain);                         //0x000D000B
    bool setDuckerDepth(const QString& itemName, double gain);                          //0x000D000C
    bool getDuckerInGain(const QString& itemName, double& gain);                        //0x000D000D
    bool setDuckerInGain(const QString& itemName, double gain);                         //0x000D000E
    bool getDuckerPriorityGain(const QString& itemName, double& gain);                  //0x000D000F
    bool setDuckerPriorityGain(const QString& itemName, double gain);                   //0x000D0010
    bool getDuckerOutGain(const QString& itemName, double& gain);                       //0x000D0011
    bool setDuckerOutGain(const QString& itemName, double gain);                        //0x000D0012
    bool getDuckerBypass(const QString& itemName, bool& bypass);                        //0x000D0013
    bool setDuckerBypass(const QString& itemName, bool bypass);                         //0x000D0014

    //================淡入淡出模块================//
    bool getGainRampTime(const QString& itemName, quint8 type, quint16& ms);            //0x000E0001
    bool setGainRampTime(const QString& itemName, quint8 type, quint16 ms);             //0x000E0002
    bool getGainRampGain(const QString& itemName, quint8 type, double& gain);           //0x000E0003
    bool setGainRampGain(const QString& itemName, quint8 type, double gain);            //0x000E0004
    bool getGainRampType(const QString& itemName, quint8& type);                        //0x000E0005
    bool setGainRampType(const QString& itemName, quint8 type);                         //0x000E0006

    //================播放器模块================//
    bool getPlayerState(const QString& itemName, quint8& state);                        //0x000F0001
    bool setPlayerState(const QString& itemName, quint8 state);                         //0x000F0002

    //==================场景==================//
    bool getCurrentSceneNum(quint8& sceneNum);                                          //0xFFFF0002
    bool setCurrentSceneNum(quint8 sceneNum);                                           //0xFFFF0003

protected:
    QByteArray formatDatagram(const UdpGram& udpGram);
    bool sendAndReceiveCommand(const UdpGram& sendGram, UdpGram& recvGram);

private:
    QUdpSocket *_udpDevice;
    QHostAddress _devIpAddress;
///////////////////////////////////////////////////////
/// /// 实现Singleton模式相关代码
public:
    static CenterCtrl *instance();
private:
    CenterCtrl();//禁止构造函数。
    CenterCtrl(const CenterCtrl&);//禁止拷贝构造函数。
    CenterCtrl & operator=(const CenterCtrl&);//禁止赋值拷贝函数。
    static QMutex Mutex;//实例互斥锁。
    static CenterCtrl* _Instance; //<使用原子指针,默认初始化为0。
};


#endif // CENTERCTRL_H
