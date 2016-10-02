/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: dragpoint.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ���ŵ�
**Others: 1���������ſؼ�
**
**�޸���ʷ:
**20160928:wey:�����游�ؼ���ת�Ƕȱ仯����̬�л���ͣ�������ʽ
**20160930:wey:���������QGraphicsPolygonItem
*************************************************/
#ifndef DRAGPOINT_H
#define DRAGPOINT_H

#include "mysuperitem.h"
#include "ItemHeader.h"

class MyItem;

class DragPoint : public MySuperItem
{
    Q_OBJECT
public:
    DragPoint(const PointType pointType,GraphicsType type, MyItem * parent = 0 ,QObject * parent1 = 0);
    ~DragPoint();
    void updatePos(PointType type,QPointF point);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updateDragPointHoverCursor(qreal parentRotateDegree);

signals:
    void currMouseState(MouseType,PointType,QPointF);
    void resizeItemSize();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    PointType pointType;
    Qt::CursorShape cursorShape;
};

#endif // DRAGPOINT_H
