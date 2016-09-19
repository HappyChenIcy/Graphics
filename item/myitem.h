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
**
*************************************************/
#ifndef MYITEM_H
#define MYITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPen>

#include "ItemHeader.h"

class QGraphicsItem;
class DragPoint;
class RotateLine;
class QMenu;
class MyArrow;
class MyTextItem;
class MyNodePort;

#include <QDataStream>

#include "dragpoint.h"
#include "rotateline.h"
#include "../Header.h"

class MyItem : public QObject,public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    MyItem(GraphicsType itemType,QMenu * menu,QGraphicsScene * parentScene,QObject * parent1 = 0,QGraphicsItem * parent2 = 0);
    ~MyItem();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    GraphicsType getType(){return this->currItemType;}
    void updateRotation(int rotateValue);

    QString getText();
    void setText(QString text);

    void setPos(const QPointF &pos);
    void setZValue(qreal z);

    void resetPolygon();                   //�����趨�����
    void setDragPointVisible(bool flag);

    friend QDataStream & operator <<(QDataStream &,MyItem * item);
    friend QDataStream & operator >>(QDataStream &,MyItem * item);

signals:
    void updateSceneDraw();
    void propHasChanged(ItemProperty);

private slots:
    void procResizeItem();
    void procRotate(MouseType mouseType, int degree);
    void procMouseState(MouseType, PointType pointType, QPointF currPos);
    void procPortChanged(MouseType type, QPointF currPoint);

    void procDeleteNodePort(MyNodePort * nodePort);
    void procEditNodePort(MyNodePort * nodePort);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    void updateRotateLinePos();
    void procResizeNodePort();
    qreal getPointToRectMinDistance(QRectF rect,QPointF point);
    void getRangeValue(qreal maxValue,qreal minValue,qreal & currValue);

    int radius;

    GraphicsType currItemType;

    QRectF boundRect;

    QPen selectedPen;
    int selectedPenWidth;

    DragPoint * leftTopPoint;
    DragPoint * rightTopPoint;
    DragPoint * leftBottomPoint;
    DragPoint * rightBottomPoint;

    DragPoint * topPoint;
    DragPoint * leftPoint;
    DragPoint * rightPoint;
    DragPoint * bottomPoint;

    MyTextItem * myTextItem;           //������Ϣ

    RotateLine * rotateLine;
    QGraphicsScene * parentScene;      //���ڵ�����

    QMenu * rightMenu;

    MouseType currMouseType;

    QList<MyArrow *> arrows;           //������ӵļ�ͷ
    QPolygonF itemPolygon;             //��ǰͼ�εĸ����������꼯��

    QList<MyNodePort*> ports;          //��ǰ�ؼ��˿ڵļ���

    ItemProperty property;             //���浱ǰ����

    bool isNeedBorder;                 //�Ƿ���Ҫѡ�б߿�

};

#endif // MYITEM_H
