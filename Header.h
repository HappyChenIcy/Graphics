/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: Header.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ����ͨ�õĽṹ��
**Others:
**
**�޸���ʷ:
**20160913:wey:�������͸�ʽ����Ϊqreal
**20160925:wey:�������ؿؼ�λ�ù�ϵSpiltDirect
**20160927:wey:�����������AddLineType
**             �����������Ϊƽ���ı���
*************************************************/
#ifndef HEADER_H
#define HEADER_H

#define PI 3.141592653
#define ICON_WIDTH  60          //ͼ��Ŀ��
#define ICON_HEIGHT 60
#define LAYOUT_SPACING  1       //���ֹ����������ؼ���ľ���
#define FONT_DEAFULT_PIX  9     //�����ʼ����С
#define ROTATE_MIN_DEGREE 0     //��ת����С�Ƕ�
#define ROTATE_MAX_DEGREE 360   //��ת�����Ƕ�

#include <QString>

#if defined(Q_CC_MSVC)
#include "typeinfo.h"
#elif defined(Q_CC_GNU)
#include "typeinfo"
#endif

#define TYPE_ID(a) typeid(a).name()
#define MY_ASSERT(a) {if(!a) return;}

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDataStream>
#include <QUuid>

#include "./item/ItemHeader.h"

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
    GRA_SQUARE,          //������
    GRA_RECT,            //����
    GRA_ROUND_RECT,      //Բ�Ǿ���
    GRA_CIRCLE,          //Բ
    GRA_ELLIPSE,         //��Բ
    GRA_POLYGON,         //����
    GRA_PARALLELOGRAM,   //ƽ���ı���
    GRA_LOOP_UP,         //ѭ������
    GRA_LOOP_DOWN,       //ѭ������
    GRA_ANNOTATION,      //ע��
    GAR_PARALLE,         //����
    GRA_LINE,            //ֱ��
    GRA_VECTOR_LINE,     //����
    GRA_TEXT,            //�ı�
    GRA_NODE_PORT,       //�˿�
    GRA_DRAG_POINT,      //�ؼ���ק��
    GRA_DRAG_LING,       //�ؼ��߶���ק��
    GRA_ROTATE_POINT,    //�ؼ���ת��
    GRA_ITEM_INFO        //�ؼ���ʾ��Ϣ
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

//�˿ڵ������˺��������ֵ�����һ��
enum DragRange
{
    DRAG_RANG_NONE,
    DRAG_RANG_MAX,
    DRAG_RANG_MIN
};

//�ָ�����ؼ���λ�ù�ϵ
enum SpiltDirect
{
    SPLIT_NONE,
    SPLIT_LEFT,
    SPLIT_TOP,
    SPLIT_RIGHT,
    SPLIT_BOTTOM
};

//������״̬
enum ItemLockState
{
    ITEM_ALL_LOCK,             //ȫ������
    ITEM_NOT_ALL_LOCK,         //��������/����δ����
    ITEM_ALL_UNLOCK            //ȫ��δ����
};

//����������
enum AddLineType
{
    LINE_HORIZONTAL,           //ֱ��
    LINE_ARROW,                //��ͷ
    LINE_SOLID_TRIANGLE        //ʵ�����Ǽ�ͷ
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
        isMoveable = true;
        rotateDegree = 0;
        itemPen.setWidth(1);
        alphaValue = 100;
        zValue = 0;
        startLineType = LINE_HORIZONTAL;
        endLineType = LINE_HORIZONTAL;
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

    bool isNeedBrush;               //Ϊtureʱ��ʾ��Ҫ��ˢ���
    QBrush itemBrush;
    bool isNeedBorder;              //Ϊtrueʱ��ʾ�Ƿ���Ҫ�߿�
    QPen   itemPen;

    MyRect itemRect;
    int    alphaValue;              //͸����ֵ
    int    rotateDegree;

    bool isFont;                    //�Ƿ�Ϊ����
    QString content;                //��������
    QFont  itemFont;
    QColor fontColor;               //������ɫ

    qreal zValue;                   //���ֵ

    LineType lineType;              //����������
    AddLineType startLineType;      //��ʼ��������
    AddLineType endLineType;        //��ֹ��������
    PointType startPointType;       //�����ڡ�LINE_MYITEM��ģʽ�£��������item�ߵ�λ��
    PointType endPointType;         //�����ڡ�LINE_MYITEM��ģʽ�£��յ������item�ߵ�λ��

    bool isMoveable;                //�Ƿ�����
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
