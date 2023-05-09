#ifndef CONFIGMIXERPUSHBUTTON_H
#define CONFIGMIXERPUSHBUTTON_H

#include <QPushButton>
#include <QSlider>

class ConfigMixerPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ConfigMixerPushButton(QWidget *parent = 0);
    void init(int value);

signals:
    void gainValueChanged(int);

protected:
    void contextMenuEvent(QContextMenuEvent *);

public slots:
    void buttonDispChanged(int value);

private:
    QSlider *slider;
    QMenu *menu;
};

#endif // CONFIGMIXERPUSHBUTTON_H
