/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myitem.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: �Զ�����ӿؼ�����
**Others: 1��֧�ֶ������Ϳؼ��Ĵ���
**        2��֧���Ҽ��˵��¼�
**        3��֧�ֶ���ʽ���޸�
**        4��֧���ƶ�����ק�����š���ת�ȹ��ܣ��������⣩
**        5��֧�����ؼ���ļ�ͷ���ߣ�֧��ɾ��
**
**�޸���ʷ:
**20160908:wey:�����������
**             ��ӶԿؼ���קʵʱ��ʾλ�ã�ͬʱ֧�ֲ˵������ÿؼ�λ��
**20160909:wey:���ӶԶԵ�ǰ�������������أ���Ҫ�����ļ����档
**20160912:wey:���ӶԿؼ��С����ҡ�������
**20160913:wey:���ӶԿؼ���б�ϡ���б�ϡ���б�¡���б������
**             �޸�����ͽ���ʱ��δ�ܰ������ź�ĳߴ���ʾ
**20160918:wey:���Ӷ˿����룬֧���ƶ��Ͱ�������
**20160919:wey:���Ӷ˿���ʽ�ͱ��ؼ���ˢ��
**             �޸����ƿؼ�ʱ����С���Ḵ������
**             ����ɾ��ʱͬ��ɾ���˿ڼ���
**             ���ӳߴ�����ƶ��˿ڸ��¶˿�����
**             ��������״̬�л�������ʱѡ�пؼ�����ȡ��ѡ��
**20160920:wey:���Ӷ�����˿ڱ��λ��ʮ����
**20160921:wey:�޸��ƶ��ؼ����޸���ʽλ�÷���΢Сƫ��
**20160922:wey:��ӿؼ���������������
**20160923:wey:��Ӷ˿��ĸ��������ƶ����ܡ���������
**20160927:wey:���ƽ���ı���
**             �޸���������קʱ��״�ı�����
**             ���ѭ�����ޡ�ѭ�����ޡ����С�ע��
**20160930:wey:�����ؼ��Ľǲ����������ţ�ȡ���ı��е�����
**             ����ı��е����������߻���
**20161003:wey:����ƽ���ı��Ρ�ע�͡����пؼ�������������ʾλ��
**20161010:wey:��ӿؼ�����
**20161017:wey:�������״̬����ʾ����/����ģʽ
**          gx:�����ؼ���ת����ÿ1����ת�ĳ�ÿ5����ת(�ɵ���)
**          gx:��ӶԿؼ���С�ߴ���޶��������߽�Ϊ����
**20161018: gx:�޸���ת��ߴ����ŵ����⡾!!!��
**             �޸����ش򿪺���ק�������ʽ����Ƕȸı�����
*************************************************/
#ifndef MYITEM_H
#define MYITEM_H

#include <QPolygonF>
#include <QPen>

#include "ItemHeader.h"
#include "mysuperitem.h"

class QGraphicsItem;
class DragPoint;
class RotateLine;
class MyArrow;
class MyTextItem;
class MyNodePort;
class DragLinePoint;

#include <QDataStream>

#include "../Header.h"

class MyItem : public MySuperItem
{
    Q_OBJECT
public:
    MyItem(GraphicsType itemType,QGraphicsScene * parentScene,QObject * parent1 = 0,QGraphicsItem * parent2 = 0);
    ~MyItem();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setProperty(ItemProperty property);

    void updateRotation(int rotateValue);

    QString getText();
    void setText(QString text);

    const QList<MyNodePort *> & getNodePorts(){return this->ports;}

    MyNodePort *addNodePort(const NodePortProperty & prop);
    void removeNodePort(MyNodePort * port);

    void setPos(const QPointF &pos);
    void setZValue(qreal z);

    void resetPolygon();                   //�����趨�����
    void setDragPointVisible(bool flag);
    void setDragLineVisible(bool isVisible);
    void removeDragLineArrows();

    void setMoveable(bool lockState);
    bool isMoveable(){return this->property.isMoveable;}

    DragLinePoint * getDragLinePoint(PointType pointType);
    QList<MyArrow *> getArrows();

    void resetItemUUID();                  //�����趨�ؼ���UUID��ţ�ͬʱ�����ӽڵ�ĸ�����ֵ

    friend QDataStream & operator <<(QDataStream &,MyItem * item);
    friend QDataStream & operator >>(QDataStream &,MyItem * item);

    ServiceProperty * getServiceProp(){return this->serviceProp;}

    void hightLightItem(HightLightLevel level = LEVEL_NORMAL,bool isHigh = true);
    bool isHightShow(){return this->isSimulateHigh;}

signals:
    void updateSceneDraw();
    void editMe();
    void propHasChanged(ItemProperty);
    void itemPosChanged(MyItem  * );
    void itemRotationChanged(MyItem  *);
    void clearOtherSelectedItems(MyItem  *);

private slots:
    void procResizeItem();
    void procRotate(MouseType mouseType, qreal degree);
    void procMouseState(MouseType, PointType pointType, QPointF currPos);
    void procDragLineMouseState(MouseType);
    void procPortChanged(MouseType type, QPointF currPoint);

    void procDeleteNodePort(MyNodePort * nodePort);
    void procEditNodePort(MyNodePort *nodePort);
    void procUpdateTextGeometry();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void updateRotateLinePos();
    void procResizeNodePort();
    void getRealTimePos();
    void updateDragPointCursor();
    qreal getPointToRectMinDistance(QRectF rect,QPointF point);
    bool getRangeValue(qreal maxValue, qreal minValue, qreal & currValue, DragRange &range);
    MyNodePort * createProp(GraphicsType type, const QPointF pos, const DragDirect direct, const qreal scaleFactor);
    DragDirect getDropDirect(const QPointF &currPoint);
    qreal getLoopMaxSidLength(qreal width, qreal height);
    void setInitalPolygon(QRectF boundRect, qreal tx, qreal ty, qreal tw, qreal th);
    void initComponentItem();

    QPen selectedPen;
    int selectedPenWidth;

    DragPoint * leftTopPoint;
    DragPoint * rightTopPoint;
    DragPoint * leftBottomPoint;
    DragPoint * rightBottomPoint;

    DragLinePoint * topLinePoint;
    DragLinePoint * leftLinePoint;
    DragLinePoint * rightLinePoint;
    DragLinePoint * bottomLinePoint;

    MyTextItem * myTextItem;           //������Ϣ

    RotateLine * rotateLine;
    QGraphicsScene * parentScene;      //���ڵ�����

    MouseType currMouseType;

    QList<MyNodePort*> ports;          //��ǰ�ؼ��˿ڵļ���

    bool isNeedBorder;                 //�Ƿ���Ҫѡ�б߿�
    bool isDragging;                   //�Ƿ������룬��������ڱ߿���ʾ�����
    DragDirect dragMoveDirect;         //�϶�ʱ��������ĸ�������
    QPointF dragMovePoint;             //������ʱ�����ڼ�¼�����ƶ��ĵ�

    bool isPrepareLine;                //�Ƿ�׼����������������ǣ�����������ʱ�����ıߵ��е������������

    ServiceProperty * serviceProp;     //������Ϣ

    bool isSimulateHigh;               //�Ƿ�Ϊ����״̬�µĸ�����ʾ
    HightLightLevel  lightLevel;       //��ǰ����������ʾ

    GraphicsType dragGraphicsType;     //��ǰ����Ķ˿����ͣ���ק��������ΪGRA_NONE
};

#endif // MYITEM_H
