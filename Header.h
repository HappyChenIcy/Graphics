/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: Header.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ����ͨ�õĽṹ��
**Others:
**
**�޸���ʷ:
**20160913:wey:�������͸�ʽ����Ϊqreal
*************************************************/
#ifndef HEADER_H
#define HEADER_H

#define PI 3.141592653
#define ICON_WIDTH  60    //ͼ��Ŀ��
#define ICON_HEIGHT 60

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
    GRA_TEXT,
    GRA_NODE_PORT
};

//����˿ڵķ���
enum DragDirect
{
    DRAG_NONE,
    DRAG_LEFT,
    DRAG_TOP,
    DRAG_RIGHT,
    DRAG_BOTTOM
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

    qreal x;
    qreal y;
    qreal width;
    qreal height;
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

        createUUID();
    }

    //����һ���µ����Ψһ��ʶ���ڼ��С����ơ����ش�ʱ��Ҫ�ֶ��ĸ��´�ֵ
    void createUUID()
    {
        startItemID = QUuid::createUuid().toString();    //�½�����ʱ����Ψһ��ʶ��
    }

    QString startItemID;                  //�����ڷ�ֱ�ߵĿؼ�ʱ��ֻ��startItemId��ʶ��ǰ�ؼ�
    QString endItemID;                    //������ֱ�߿ؼ�ʱ��������ʾֱ���������ӵĿؼ���ID��

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

    qreal zValue;            //���ֵ
};

//�ڵ�����
struct NodePortProperty
{
    NodePortProperty()
    {
        startItemID = QUuid::createUuid().toString();    //�½�����ʱ����Ψһ��ʶ��
    }

    friend QDataStream & operator <<(QDataStream &,NodePortProperty & item);
    friend QDataStream & operator >>(QDataStream &,NodePortProperty & item);

    QBrush itemBrush;           //�����ɫ
    DragDirect direct;          //����ķ���
    qreal scaleFactor;          //�����λ���൱�ڵ�ǰһ�����ڵı���
    QString parentItemID;       //���ؼ���ID(������Ѱ��ͣ����)
    QString startItemID;        //�����ڷ�ֱ�ߵĿؼ�ʱ��ֻ��startItemId��ʶ��ǰ�ؼ�(���ڼ�ͷѰ�Ҹ��ڵ�)
};

//�ݴ�һ������ʱ�Ŀؼ���Ϣ
struct CutInfo
{
    GraphicsType graphicsType;
    ItemProperty itemProperty;
    QString content;
    QList<NodePortProperty> nodeProperties;      //�ؼ������˿ڵ���Ϣ
};

#endif // HEADER_H
