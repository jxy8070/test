#ifndef SKINSTYLE_H
#define SKINSTYLE_H

enum SkinType
{
    Skin_None = -1,
    Skin_FangTu = 1,
    Skin_KemuWhite,
    Skin_KemuBlack,
    Skin_KemuGrey,
    Skin_Default,
};


class SkinStyle : QObject
{
    Q_OBJECT
public:
    SkinStyle(SkinType skinType);

    QString getSkinIconDir();
    const QColor getConfigItemColor(CfgType type);
    inline SkinType skinType() {return _skinType;}

public:
    static QString getAppStyleSheet();
    static void loadAppTranslator(QApplication& app);
    static void loadAppFont();
    static QString getCfgSettingStyleSheet();
    static QString getSkinName(SkinType skinType);
    static const QColor getConfigItemColor_FangTu(CfgType type);
    static const QColor getConfigItemColor_Default(CfgType type);
    static const QFont getFont(int size, bool isBord = false);

public:
    QColor SceneFillColor;
    QColor LineColor;
    QColor DevItemTitilColor;
    QColor VolumnLableColor;
    QColor OnlineTextColor;


private:
    SkinType _skinType;

};

#endif // SKINSTYLE_H
