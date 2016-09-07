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
    GRA_TEXT,
    GRA_ARROW
};

//��¼��ǰitem��scene�е�x��y��w��hֵ
struct MyRect
{
    MyRect()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }
    int x;
    int y;
    int width;
    int height;
};

//ÿ��itemӵ�е����ԣ���ˢ�����ʡ�λ�á���С���Ƕȡ�����
struct ItemProperty
{
    ItemProperty()
    {
        isNeedBrush = true;
        isNeedBorder = true;
        isFont = false;
        rotateDegree = 0;
        itemPen.setWidth(1);
        alphaValue = 100;
    }

    bool isNeedBrush;        //Ϊtureʱ��ʾ��Ҫ��ˢ���
    QBrush itemBrush;

    bool isNeedBorder;       //Ϊtrueʱ��ʾ�Ƿ���Ҫ�߿�
    QPen   itemPen;
    QFont  itemFont;
    MyRect itemRect;
    int    alphaValue;       //͸����ֵ
    int    rotateDegree;

    bool isFont;             //�Ƿ�Ϊ����
};

#endif // HEADER_H
