#include "util.h"

#include <QWidget>
#include <QUuid>

#include "global.h"

void Util::setWidgetColor(QWidget *widget, QColor &color)
{
    widget->setStyleSheet("background-color:rgba("+QString::number(color.red())\
                          +","+QString::number(color.green())\
                          +","+QString::number(color.blue())\
                          +","+QString::number(color.alpha())+")");
}

//��ȡ�������Ϣ
QString Util::getFontInfo(QFont font)
{
    return QString("%1px,%2").arg(font.pointSize()).arg(font.family());
}

//����ÿ���ؼ���������ȣ�����ӻ���ʱ���Զ�����
qreal Util::getGlobalZValue()
{
    GlobalItemZValue += 0.1;
    return GlobalItemZValue;
}

//����տؼ�ʱ�Զ���0
void Util::resetGlobalZValue()
{
    GlobalItemZValue = 0;
}

//��ȡΨһ��ʶ��
QString Util::getUUID()
{
    return QUuid::createUuid().toString();
}

qreal Util::getMaxNum(qreal &a, qreal &b)
{
    if(a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
    return 0;
}
