#ifndef HEADER_H
#define HEADER_H

#include <QBrush>
#include <QPen>
#include <QFont>

//��ǰ��ӵ�����
enum GraphicsType
{
    GRA_NONE,
    GRA_SQUARE,
    GRA_RECT,
    GRA_ROUND_RECT,
    GRA_CIRCLE,
    GRA_ELLIPSE,
    GRA_POLYGON,
    GRA_LINE,
    GRA_TEXT
};

//ÿ��itemӵ�е����ԣ���ˢ�����ʡ�λ�á���С���Ƕȡ�����
struct ItemProperty
{
    ItemProperty()
    {
        isNeedBrush = true;
        isNeedBorder = true;
        rotateDegree = 0;
        itemPen.setWidth(1);
        alphaValue = 100;
    }

    bool isNeedBrush;        //Ϊtureʱ��ʾ��Ҫ��ˢ���
    QBrush itemBrush;

    bool isNeedBorder;       //Ϊtrueʱ��ʾ�Ƿ���Ҫ�߿�
    QPen   itemPen;
    QFont  itemFont;
    QRect  itemRect;
    int    alphaValue;       //͸����ֵ
    int    rotateDegree;
};

#endif // HEADER_H
