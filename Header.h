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

#define M_VERTION 0x000A        //����İ汾���ڱ����ļ�ʱ��Ҫ���浱ǰ�ļ��İ汾������ʱҲҪ�ж�
#define M_AUTHOR "�Ͼ��ʹ�ϵͳ�������޹�˾"

//���ڿ����Ƿ���Ҫ����״̬��ģ�飬�������Ҫ�����˺�ȡ������
#ifndef ADD_STATE_MODEL
#define ADD_STATE_MODEL
#endif

#define PI 3.141592653
#define ICON_WIDTH  60          //��קͼ��Ŀ��
#define ICON_HEIGHT 60
#define LAYOUT_SPACING  1       //���ֹ����������ؼ���ľ���
#define FONT_DEAFULT_PIX  9     //�����ʼ����С
#define ROTATE_MIN_DEGREE 0     //��ת����С�Ƕ�
#define ROTATE_MAX_DEGREE 360   //��ת�����Ƕ�

#define POP_SUB_DIALOG_WIDTH  650    //������������Ӵ��ڹ̶����
#define POP_SUB_DIALOG_HEIGHT 450    //������������Ӵ��ڹ̶��߶�
#define POP_SIMULATE_DIALOG_WIDTH 550       //���������༭����
#define POP_SIMULATE_DIALOG_HEIGHT 700

#define COMBOX_START_FLAG "[QUOTE"   //���Ա༭ֵ������ʼ��ʶ
#define COMBOX_LOOP_QUOTE "[QUOTE]"  //ѭ�����Ա༭ʱ������ǰ������ı���

#define COMBOX_STYLESHEET "QComboBox QAbstractItemView::item{height:23px;}\
                            QComboBox QAbstractItemView::item:hover{background-color:#567dbc;}"

#include <QString>

#if defined(Q_CC_MSVC)
#include "typeinfo.h"
#elif defined(Q_CC_GNU)
#include "typeinfo"
#endif

#define TYPE_ID(a) typeid(a).name()
#define MY_ASSERT(a) {if(!a) return;}
#define MY_ASSERT_RETURN_NULL(a) {if(!a) return NULL;}
#define MY_ASSERT_GT_ZERO(a) {if(a <=0 ) return;}

#define MY_BUILD_MODEL_ONLY {if(GlobalWindowState != WINDOW_BUILD_MODEL) return;}
#define MY_SIMULATE_MODEL_ONLY {if(GlobalWindowState != WINDOW_SIMULATE) return;}

#include <QVariant>

typedef  QList<QVariant> VariantList;
typedef  QList<VariantList> DataList;

class MyItem;
class QGraphicsItem;

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
    //�˿�
    GRA_NODE_PROCESS,         //ԭ�Ӵ�����
    GRA_NODE_HALF_CIRCLE,     //D�˿�
    GRA_NODE_TRIANGLE_IN ,    //��������˿�
    GRA_NODE_TRIANGLE_OUT,    //��������˿�
    GRA_NODE_CIRCLE,          //Բ����˿�
#ifdef ADD_STATE_MODEL
    //״̬��
    GRA_STATE_START,          //״̬����ʼ
    GRA_STATE_END,            //״̬������
    GRA_STATE_PROCESS,        //״̬������
    //����
    GRA_MASK_RECT,            //��������
    GRA_MASK_BOUND_RECT,      //Բ�Ǿ�������
    GRA_MASK_CIRCLE,          //Բ������
#endif
    GRA_NO_MEAN               //������ֵ
};

//��Ԫ����
enum ProcessType
{
    PRO_NONE,               //��
    PRO_START,              //��ʼ:Բ�Ǿ���
    PRO_INPUT,              //�������:ƽ���ı���
    PRO_PROCESS,            //����:����
    PRO_JUDGE,              //�ж�:����
    PRO_PARALLEL,           //����:ƽ����
    PRO_LOOP,               //ѭ��
    PRO_END                 //����:Բ�Ǿ���
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
        ptype = PRO_NONE;
        createUUID();
    }

    //����һ���µ����Ψһ��ʶ���ڼ��С����ơ����ش�ʱ��Ҫ�ֶ��ĸ��´�ֵ
    void createUUID()
    {
        startItemID = QUuid::createUuid().toString();    //�½�����ʱ����Ψһ��ʶ��
        associativeID = QUuid::createUuid().toString();
    }

    QString startItemID;                  //�����ڷ�ֱ�ߵĿؼ�ʱ��ֻ��startItemId��ʶ��ǰ�ؼ�
    QString endItemID;                    //������ֱ�߿ؼ�ʱ��������ʾֱ���������ӵĿؼ���ID��
#ifdef ADD_STATE_MODEL
    QString associativeID;                //���ڸ������Ѱ��ԭ��ͼ��ԭ��ͼѰ��״̬ͼ�ı�ʶ,��Ϊ�ļ�����!!!��
#endif

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

    ProcessType ptype;              //����ģʽ���������
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
    QString content;            //�ڵ�������Ϣ
    QBrush itemBrush;           //�����ɫ
    DragDirect direct;          //����ķ���
    qreal scaleFactor;          //�����λ���൱�ڵ�ǰһ�����ڵı���
    QString parentItemID;       //���ؼ���ID(������Ѱ��ͣ����)
    QString startItemID;        //�����ڷ�ֱ�ߵĿؼ�ʱ��ֻ��startItemId��ʶ��ǰ�ؼ�(���ڼ�ͷѰ�Ҹ��ڵ�)
};

/****************************************���Ա༭**************************************************/
//�������������
struct Parameter
{

    friend QDataStream& operator <<(QDataStream &,Parameter * rect);
    friend QDataStream& operator >>(QDataStream &,Parameter * rect);

    QString pId;               //����ֵ
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

    friend QDataStream & operator <<(QDataStream &,ServiceProperty * item);
    friend QDataStream & operator >>(QDataStream &,ServiceProperty * item);

    bool hasSettInfo;                 //�Ƿ��Ѿ���������Ϣ����������ʾ���Ա༭ʱ��Ϊ��ʶ�Ƿ�Ҫ�����ݿ��ȡ
    QString id;                       //���ݿ�����ֵ
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

enum LoopPart
{
    LOOP_VARI,          //ѭ����������
    LOOP_EXPRESS,       //ѭ�����ʽ
    LOOP_FINAL          //ѭ��ĩβ
};

//��������int i = 0;
struct VariableDefine
{
    friend QDataStream & operator <<(QDataStream & stream,VariableDefine *);
    friend QDataStream & operator >>(QDataStream & stream,VariableDefine *);

    QString type;              //int
    QString name;              //i
    QString value;             //0
    QString pRemark;           //��ע
};

typedef QList<VariableDefine *> VariableDefineList;

//���ʽ����i < 5
struct ExpressDefine
{
    friend QDataStream & operator <<(QDataStream & stream,ExpressDefine *);
    friend QDataStream & operator >>(QDataStream & stream,ExpressDefine *);

    QString name;              //i
    QString expressType;       //<
    QString value;             //5
    QString pRemark;           //��ע
};

typedef QList<ExpressDefine *> ExpressDefineList;

//ĩѭ���嶨��i++/i+=4
struct FinalExpressDefine
{
    friend QDataStream & operator <<(QDataStream & stream,FinalExpressDefine *);
    friend QDataStream & operator >>(QDataStream & stream,FinalExpressDefine *);

    QString name;             //i
    QString expressType;      //++
    QString value;            //4
    QString pRemark;          //��ע
};

typedef QList<FinalExpressDefine *> FinalExpressDefineList;

//����������ӳ���ϵint i = 0;i<5;i++
struct SignalVari
{
    SignalVari()
    {
       variName = "";
       isLegal = false;
       isQuoted = false;
       isAssignedValue = false;
       initialValue = 0;
       finalValue = 0;
       middlValue = 0;
       operateSymbol = "";
       selfOperateSymbol = "";
       selfOperateValue = 0;
    }

    friend QDataStream & operator <<(QDataStream & stream,SignalVari *);
    friend QDataStream & operator >>(QDataStream & stream,SignalVari *);

    QString variName;           //������i
    bool isQuoted;              //�Ƿ�Ϊ����ֵ,��Ϊ����ֵ��ôinitialValue��middlValueֵ��Ҫ�����������ͨ����ȡǰ��ķ����������г�ʼ��
    bool isAssignedValue;       //��isQuotedΪtrueʱ����ֵ��Ч�����Ϊfalse������Ҫ��ȡ���õ�ֵ֮����Ϊtrue��
    bool isLegal;               //�Ƿ�Ϸ��������ĳ������ֵ��������ô������Ϊfalse����ʹ��ʱ���Ϊfalse��ô�Ͳ�ʹ��
    int initialValue;           //��ʼֵ0
    int finalValue;             //��ֹ����ֵ5
    int middlValue;             //�м�ֵ
    QString operateSymbol;      //������������<��>��<=...
    QString selfOperateSymbol;  //����++��--��+=
    int selfOperateValue;       //����ֵ(+=2����ʾ2)
    QList<int> middleResults;   //ѭ�������в������м�ֵ<key:�ڼ���ѭ����value����ǰѭ��״̬�±�����ֵ>
};

typedef QList<SignalVari *> SignalVariList;

#include <QDebug>

//ѭ����������
struct LoopProperty
{
    friend QDataStream & operator <<(QDataStream & stream,LoopProperty *);
    friend QDataStream & operator >>(QDataStream & stream,LoopProperty *);

    ~LoopProperty()
    {
        foreach(SignalVari * tmp,signalList)
        {
            delete tmp;
        }
        signalList.clear();

        foreach(VariableDefine * tmp,varList)
        {
            delete tmp;
        }
        varList.clear();

        foreach(ExpressDefine * tmp,expList)
        {
            delete tmp;
        }
        expList.clear();

        foreach(FinalExpressDefine * tmp,fexpList)
        {
            delete tmp;
        }
        fexpList.clear();
    }

    SignalVariList signalList;          //�����������
    VariableDefineList varList;         //��������
    ExpressDefineList expList;          //���ʽ����
    FinalExpressDefineList fexpList;    //ĩѭ���弯��
};

//�жϿ�����
struct JudgeProperty
{
    friend QDataStream & operator <<(QDataStream &,JudgeProperty *prop);
    friend QDataStream & operator >>(QDataStream &,JudgeProperty * prop);

    QString express;            //���ʽ
    QString switchExpress;      //������ã��򱣴潫����ת���ɶ�Ӧ������ı��ʽ
};

//������ʾ����кź����������ݿ���index��Ӧ��ϵ
struct RowToIndex
{
    int rowNum;
    QString indexNum;
};

typedef QList< RowToIndex > RowList;

//�ݴ�һ������ʱ�Ŀؼ���Ϣ
struct CutInfo
{
    CutInfo()
    {
        hasContent = false;
        loopProp = new LoopProperty;
        judgeProp = new JudgeProperty;
    }

    bool  hasContent;                            //�Ƿ����������,�ڼ��к͸��ƺ���Ϊtrue����պ���Ϊfalse
    GraphicsType graphicsType;
    ItemProperty itemProperty;
    ServiceProperty serviceProp;                 //����������Ϣ
    LoopProperty * loopProp;                     //����ѭ��������Ϣ
    JudgeProperty * judgeProp;                   //�����ж�������Ϣ
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

/*************************ѭ���ṹ������ֲ�������******************************/
//����Ԫ
struct ProcessUnit
{
    ProcessUnit()
    {
        item = NULL;
        nextChild = NULL;
        yesChild = NULL;
        noChild = NULL;
    }
    ProcessType ptype;               //����Ԫ����
    GraphicsType gtype;              //�ؼ�������

    MyItem * item;                   //����Ԫ��Ӧ�Ŀؼ�Item
    ProcessUnit * nextChild;         //���жϿ��£�ָ���������
    ProcessUnit * yesChild;          //�жϿ�״̬�£�yes��no child����
    ProcessUnit * noChild;           //
};

#ifdef ADD_STATE_MODEL
/**************************״̬���ṹ��******************************/
//��ʼ���˿�����
struct StatePortProperty
{
    QString portName;        //�˿�����
    QString portType;        //�˿�����
};

//����/���˿�����
struct StateInOutProperty
{
    QString portName;        //�˿�����
    QString portType;        //�˿�����
    QList<StatePortProperty> props;    //�˿ڼ���
};

//��ʼ״̬����
struct StateStartProperty
{
    QString content;
};

//�ڲ��¼�����
struct StatInnerProperty
{
    QString propName;
    QString propType;
    QString propDesc;
};

//ģ��״̬����
struct StateModelProperty
{
    QString stateName;          //״̬��
    QString continueContent;    //�����¼���Ϊ
    QList<StatInnerProperty> props;   //�ڲ��¼�����
};

#endif

#endif // HEADER_H
