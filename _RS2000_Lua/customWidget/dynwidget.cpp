#include "stable.h"
#include "dynwidget.h"
#include "config.h"

const int m_areaLen = 500;
const int m_dynLen = 400;
const int m_nRound_r = 20;

DynWidget::DynWidget(QWidget *parent) :
    QWidget(parent)
{
    m_px[0] = (m_areaLen - m_dynLen) / 2;
    m_py[0] = m_areaLen - (m_areaLen - m_dynLen) / 2;
    m_px[1] = 200;
    m_py[1] = 300;
    m_px[2] = 300;
    m_py[2] = 200;
    m_px[3] = m_areaLen - (m_areaLen - m_dynLen) / 2;
    m_py[3] = (m_areaLen - m_dynLen) / 2;
    m_nselect = 0;
}

void DynWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor color;
    int temp1, temp2, temp3, temp4, i;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(0, 0, m_areaLen, m_areaLen);

    color.setRgba(qRgba(0, 0, 0, 255));
    painter.setBrush(QBrush(color, Qt::SolidPattern));
    painter.setPen(QPen(color, 2));
    painter.drawRect(0, 0, m_areaLen, m_areaLen);

    temp2 = (m_areaLen - m_dynLen) / 2;
    color.setRgba(qRgba(255, 255, 255, 255));
    painter.setPen(QPen(color, 4));
    painter.drawRect(temp2, temp2, m_dynLen, m_dynLen);
    temp1 = m_dynLen / 10;
    temp3 = m_areaLen - temp2;
    painter.setPen(QPen(color, 1));
    painter.setFont(SkinStyle::getFont(12));
    QString string;
    for(i = 1; i < 10; i++)
    {
        temp4 = temp2 + i * temp1;
        painter.drawLine(temp2, temp4, temp3, temp4);
        painter.drawLine(temp4, temp2, temp4, temp3);
    }

    for(i = 0; i < 11; i++)
    {
        temp4 = temp2 + i * temp1;

        string = QString::number(-80 + i * 8);

//        if(i == 9)
//        {
//            painter.drawText(QPoint(temp1 - 13, m_areaLen -(temp4 - 4)), string);
//            painter.drawText(QPoint(temp4 - 10, temp3 + 20), string);
//        }
//        else if(i == 10)
//        {
//            painter.drawText(QPoint(temp1 - 6, m_areaLen -(temp4 - 4)), string);
//            painter.drawText(QPoint(temp4 - 3, temp3 + 20), string);
//        }
//        else
        {
            painter.drawText(QPoint(temp1 - 20, m_areaLen -(temp4 - 4)), string);
            painter.drawText(QPoint(temp4 - 13, temp3 + 20), string);
        }
     }

     for(i = 0; i < 2; i++)
     {
        painter.setPen(QPen(color, 2, Qt::DotLine));
        painter.drawLine(m_px[1 + i], temp2, m_px[1 + i], temp3);
     }
     painter.setPen(QPen(color, 2));
     for(i = 0; i < 3; i++)
     {
        painter.drawLine(m_px[i], m_py[i], m_px[i + 1], m_py[i + 1]);
     }
     for(i = 0; i < 4; i++)
     {
         color.setRgba(qRgba(255, 255, 255, 255));
         painter.setPen(QPen(color, 2));
         color.setRgba(qRgba(0, 0, 0, 180));
         painter.setBrush(QBrush(color, Qt::SolidPattern));
         painter.drawEllipse(m_px[i] - 10, m_py[i] - 10, 20, 20);
         string = QString::number(i + 1);
         painter.setFont(SkinStyle::getFont(10, true));
         painter.drawText(QPoint(m_px[i] - 4, m_py[i] + 5), string);
     }
}

void DynWidget::resizeEvent(QResizeEvent *e)
{
    int i;
    m_wMult = (float)e->size().width() / m_areaLen;
    m_hMult = (float)e->size().height() / m_areaLen;

    for(i = 0; i < 4; i++)
    {
        m_fpxreal[i] = m_px[i] * m_wMult;
        m_fpyreal[i] = m_py[i] * m_hMult;
    }
}

void DynWidget::PressHandle(int x, int y)
{
    int i;

    for(i = 0; i < 4; i++)
    {
        m_fpxreal[i] = m_px[i] * m_wMult;
        m_fpyreal[i] = m_py[i] * m_hMult;
        if(((m_fpxreal[i] - 10 * m_wMult) <= x) &&
                ((m_fpxreal[i] + 10 * m_wMult) >= x) &&
                ((m_fpyreal[i] - 10 * m_hMult) <= y) &&
                ((m_fpyreal[i] + 10 * m_hMult) >= y))
        {
            m_nselect = i + 1;
        }
    }

//    if(m_nselect)
//    {
//        qDebug()<< "m_nselect:" << m_nselect;
//    }
}

void DynWidget::mousePressEvent(QMouseEvent *e)
{
    PressHandle(e->x(), e->y());
}

void DynWidget::ReleaseHandle()
{
    m_nselect = 0;
}

void DynWidget::mouseReleaseEvent(QMouseEvent *)
{
    ReleaseHandle();
}

void DynWidget::MoveHandle(int x, int y)
{
    int temp1, temp2;
    temp1 = qRound(x / m_wMult);
    temp2 = qRound(y / m_hMult);
    int update_flag = 0;
    if(m_nselect == 1)
    {

        if((m_px[0] == ((m_areaLen - m_dynLen) / 2)) && (m_py[0] == (m_areaLen - (m_areaLen - m_dynLen) / 2)))
        {
            if((temp1 != m_px[0]) &&
                    (temp1 >= ((m_areaLen - m_dynLen) / 2)) &&
                    (temp1 <= ((m_areaLen - m_dynLen) / 2 + m_dynLen)))
            {
                m_px[0] = temp1;
            }
            else
            {
                if((temp2 >= ((m_areaLen - m_dynLen) / 2)) && (temp2 <= ((m_areaLen - m_dynLen) / 2 + m_dynLen)))
                {
                    m_py[0] = temp2;
                }
            }
        }
        else if(m_py[0] == (m_areaLen - (m_areaLen - m_dynLen) / 2))
        {
            if((temp1 >= ((m_areaLen - m_dynLen) / 2)) &&
                    (temp1 < m_px[1]))
            {
                m_px[0] = temp1;
            }
        }
        else if(m_px[0] == ((m_areaLen - m_dynLen) / 2))
        {
            if((temp2 >= m_py[1]) && (temp2 <= ((m_areaLen - m_dynLen) / 2 + m_dynLen)))
            {
                m_py[0] = temp2;
            }
        }
        update_flag = 1;
    }
    else if(m_nselect == 2)
    {
        if((temp1 >= ((m_areaLen - m_dynLen) / 2)) &&
                (temp1 <= m_px[2] && temp1 >= m_px[0]))
        {
            m_px[1] = temp1;
        }

        if((temp2 >= m_py[2]) && (temp2 <= m_py[0]))
        {
            m_py[1] = temp2;
        }
        update_flag = 1;
    }
    else if(m_nselect == 3)
    {
        if((temp1 > m_px[1] && temp1 <= m_px[3]) &&
                (temp1 < (m_areaLen - ((m_areaLen - m_dynLen) / 2))))
        {
            m_px[2] = temp1;
        }

        if((temp2 >= m_py[3]) && (temp2 <= m_py[1]))
        {
            m_py[2] = temp2;
        }
        update_flag = 1;
    }
    else if(m_nselect == 4)
    {
        if((m_py[3] == ((m_areaLen - m_dynLen) / 2)) && (m_px[3] == (m_areaLen - (m_areaLen - m_dynLen) / 2)))
        {
            if((temp1 != m_px[3]) &&
                    (temp1 > m_px[2]) &&
                    (temp1 <= ((m_areaLen - m_dynLen) / 2 + m_dynLen)))
            {
                m_px[3] = temp1;
            }
            else
            {
                if((temp2 >= ((m_areaLen - m_dynLen) / 2)) && (temp2 <= ((m_areaLen - m_dynLen) / 2 + m_dynLen))  && (temp2 <= m_py[2]) )
                {
                    m_py[3] = temp2;
                }
            }
        }
        else if(m_py[3] == ((m_areaLen - m_dynLen) / 2))
        {
            if((temp1 != m_px[3]) &&
                    (temp1 > m_px[2]) &&
                    (temp1 <= ((m_areaLen - m_dynLen) / 2 + m_dynLen)))
            {
                m_px[3] = temp1;
            }
        }
        else if(m_px[3] == (m_areaLen - (m_areaLen - m_dynLen) / 2))
        {
            if((temp2 >= ((m_areaLen - m_dynLen) / 2)) && (temp2 <= m_py[2]))
            {
                m_py[3] = temp2;
            }
        }
        update_flag = 1;
    }
    if(update_flag)
    {
        emit changeValue(m_px, m_py);
        update();
    }
}

void DynWidget::mouseMoveEvent(QMouseEvent *e)
{
    MoveHandle(e->x(), e->y());
}

void DynWidget::setValue(int *px, int *py)
{
//    int temp1, temp2, i;
//    temp1 = (m_areaLen - m_dynLen) / 2;
//    temp2 = m_areaLen - temp1;
    for(int i = 0; i < 4; i++)
    {
        m_px[i] = px[i];
        m_py[i] = py[i];
    }

    update();

    emit changeValue(m_px, m_py);
}

int* DynWidget::In()
{
    int temp1, i;
    temp1 = (m_areaLen - m_dynLen) / 2;
    for(i = 0; i < 4; i++)
    {
        m_in[i] = qRound((float)(m_px[i] - temp1) / 2);
    }
    return m_in;
}

int* DynWidget::Out()
{
    int temp1, temp2, i;
    temp1 = (m_areaLen - m_dynLen) / 2;
    temp2 = m_areaLen - temp1;
    for(i = 0; i < 4; i++)
    {
        m_out[i] = qRound((float)(temp2 - m_py[i]) / 2);
    }
    return m_out;
}
