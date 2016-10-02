/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: draglinepoint.h
**��  ��: wey       Version: 1.0       Date: 2016.09.30
**��  ��:
**Others: ��ק�����ڵ�
**
**�޸���ʷ:
**
*************************************************/
#ifndef DRAGLINEPOINT_H
#define DRAGLINEPOINT_H

#include "mysuperitem.h"
#include "ItemHeader.h"

class MyItem;

class DragLinePoint : public MySuperItem
{
    Q_OBJECT
public:
    DragLinePoint(const PointType pointType,GraphicsType type, MyItem * parent1 = 0 ,QObject* parent = 0);
    ~DragLinePoint();

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    PointType pointType;
};

#endif // DRAGLINEPOINT_H
