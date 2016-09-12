#ifndef HEADER_H
#define HEADER_H

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDataStream>
#include <QUuid>

enum ReturnType
{
    RETURN_OK,
    FILE_NOT_EXIST,
    FILE_CANT_READ,
    FILE_CANT_WRITE,
    FILE_ILLEGAL             //�ļ���ʽ���Ϸ�
};

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
    GRA_VECTOR_LINE,
    GRA_TEXT
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

    friend QDataStream& operator <<(QDataStream &,MyRect & rect);
    friend QDataStream& operator >>(QDataStream &,MyRect & rect);

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
        zValue = 0;

        startItemID = QUuid::createUuid().toString();
    }

    QString startItemID;                  //�����ڷ�ֱ�ߵĿؼ�ʱ��ֻ��startItemId��ʶ��ǰ�ؼ�
    QString endItemID;                    //������ֱ�߿ؼ�ʱ��������ʾֱ���������ӵĿؼ�

    friend QDataStream & operator <<(QDataStream &,ItemProperty & item);
    friend QDataStream & operator >>(QDataStream &,ItemProperty & item);

    bool isNeedBrush;        //Ϊtureʱ��ʾ��Ҫ��ˢ���
    QBrush itemBrush;

    bool isNeedBorder;       //Ϊtrueʱ��ʾ�Ƿ���Ҫ�߿�
    QPen   itemPen;

    MyRect itemRect;
    int    alphaValue;       //͸����ֵ
    int    rotateDegree;

    bool isFont;             //�Ƿ�Ϊ����
    QString content;         //��������

    QFont  itemFont;
    QColor fontColor;        //������ɫ

    float zValue;            //���ֵ
};


//�ݴ�һ������ʱ�Ŀؼ���Ϣ
struct CutInfo
{
    GraphicsType graphicsType;
    ItemProperty itemProperty;
    QString content;
};

#endif // HEADER_H
