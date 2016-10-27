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
**20161009:wey:������Ա༭�ṹ��ServiceProperty
*************************************************/
#ifndef HEADER_H
#define HEADER_H

#define M_VERTION 0x0005        //����İ汾���ڱ����ļ�ʱ��Ҫ���浱ǰ�ļ��İ汾������ʱҲҪ�ж�

#define PI 3.141592653
#define ICON_WIDTH  60          //��קͼ��Ŀ��
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
#define MY_ASSERT_GT_ZERO(a) {if(a <=0 ) return;}

#define MY_BUILD_MODEL_ONLY {if(GlobalWindowState != WINDOW_BUILD_MODEL) return;}
#define MY_SIMULATE_MODEL_ONLY {if(GlobalWindowState != WINDOW_SIMULATE) return;}

#include <QVariant>

typedef  QList<QVariant> VariantList;
typedef  QList<VariantList> DataList;

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDataStream>
#include <QUuid>
#include <QDateTime>

#include "./item/ItemHeader.h"

//���ڹ�����������
enum WindowState
{
    WINDOW_BUILD_MODEL,          //ģ�ͽ���
    WINDOW_SIMULATE              //ģ������
};

//����ֵ����
enum ReturnType
{
    RETURN_ERROR = 0,                //����
    RETURN_SUCCESS = 1,              //��ȷ
    SCENE_NOT_EXIST = 2,             //����������
    SCENE_EMPTY = 3,                 //����Ϊ��
    SCENE_WITHOUT_START = 4,         //�����޿�ʼ
    SCENE_MORE_START = 5,            //���������ʼ
    SCENE_WITHOUT_END = 6,           //�����޽���
    SCENE_MORE_END = 7,              //�����������
    SCENE_FLOW_ERROR = 8,            //�������̴���

    FLOW_NOT_COMPLETE = 10,          //���̲�����
    FLOW_COMPLETE = 12,              //��������
    FLOW_SUCCESS = 13,               //�����߼���ȷ
    FLOW_ERROR = 14,                 //�����߼�����

    FILE_NOT_EXIST = 30,             //�ļ�������
    FILE_CANT_READ = 31,             //�ļ����ɶ�
    FILE_CANT_WRITE = 32,            //�ļ�����д
    FILE_ILLEGAL = 33,               //�ļ���ʽ���Ϸ�
    FILE_VERSION = 34                //�ļ��İ汾��һ��(�ļ������汾��һ��)

};

/****************************************��ģ**************************************************/

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
    GRA_ITEM_INFO,       //�ؼ���ʾ��Ϣ
    GRA_NODE_HALF_CIRCLE,  //D�˿�
    GRA_NODE_TRIANGLE_IN ,    //��������˿�
    GRA_NODE_TRIANGLE_OUT,    //��������˿�
    GRA_NODE_CIRCLE           //Բ����˿�
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

//�ı���Ϣ���ڵķ�ʽ
enum TextExistType
{
    TEXT_ALONG,                //��������
    TEXT_CHILD                 //��Ϊĳ���ؼ��ӿؼ���������
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

    GraphicsType portType;      //�ڵ�����
    QBrush itemBrush;           //�����ɫ
    DragDirect direct;          //����ķ���
    qreal scaleFactor;          //�����λ���൱�ڵ�ǰһ�����ڵı���
    QString parentItemID;       //���ؼ���ID(������Ѱ��ͣ����)
    QString startItemID;        //�����ڷ�ֱ�ߵĿؼ�ʱ��ֻ��startItemId��ʶ��ǰ�ؼ�(���ڼ�ͷѰ�Ҹ��ڵ�)
};

//�ݴ�һ������ʱ�Ŀؼ���Ϣ
struct CutInfo
{
    CutInfo()
    {
        hasContent = false;
    }
    bool  hasContent;                            //�Ƿ����������,�ڼ��к͸��ƺ���Ϊtrue����պ���Ϊfalse
    GraphicsType graphicsType;
    ItemProperty itemProperty;
    QString content;
    QList<NodePortProperty> nodeProperties;      //�ؼ������˿ڵ���Ϣ
};

//�����ȼ�
enum HightLightLevel
{
    LEVEL_NORMAL,        //��ͨ(��)
    LEVEL_MIDDLE,        //�м�(��)��������
    LEVEL_HIGH           //�߼�(��)��������ѡ��
};

/****************************************���Ա༭**************************************************/
//�������������
struct Parameter
{
    QString pName;             //������
    QString pType;             //��������
    QString pValue;            //����ֵ
    QString pRemark;           //��ע
};

typedef  QList<Parameter *> ParaList;

//���������
struct ServiceProperty
{
    ServiceProperty()
    {
        hasSettInfo = false;
    }
    bool hasSettInfo;                 //�Ƿ��Ѿ���������Ϣ����������ʾ���Ա༭ʱ��Ϊ��ʶ�Ƿ�Ҫ�����ݿ��ȡ
    QString serviceName;              //������
    QString status;                   //����״̬
    QString descirption;              //��������
    QString servicePath;              //����·��
    QString method;                   //���ýӿ�
    ParaList inputParas;              //�����������
    ParaList outputParas;             //�����������
    bool isAsNextInput;               //����Ƿ���Ϊ�¸����������
};

//����Ļ�����Ϣ
struct ServiceInfo
{
    QString id;           //��������
    QString code;
    QString name;
    QString descirption;
    QString status;
    QString auditOpinion;
    QString method;
    QString input;
    QString result;
    int num;
    QDateTime addTime;
    QString addUser;
    QString auditUser;     //������
    QString auditUserName; //����������
    QString outputName;
    QString outputType;
    QString addUserName;
    QDateTime auditTime;
};

//����Ĳ�����Ϣ
struct ServiceParameter
{
    QString id;
    QString relationId;
    QString parameterName;
    QString parameterType;
    QDateTime addTime;
};

//����Ĳ�����Ϣ
struct ServiceDeploy
{
    QString id;
    QString relationId;
    QString hostIP;
    QString hostId;
    QDateTime addTime;
    QString exampleName;
    QString url;
    QString status;
};

typedef QList<ServiceInfo> ServiceInfoList;

//������ʾ����кź����������ݿ���index��Ӧ��ϵ
struct RowToIndex
{
    int rowNum;
    QString indexNum;
};

typedef QList< RowToIndex > RowList;

#endif // HEADER_H
