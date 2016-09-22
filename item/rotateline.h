/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: rotateline.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: �ؼ���ת���Ƶ�
**Others: 1�����ڿ��ƿؼ�����ת
**
**�޸���ʷ:
**20160922:wey:���״̬������������ͼ��ı�
*************************************************/
#ifndef ROTATELINE_H
#define ROTATELINE_H

#include <QGraphicsObject>
#include <QPixmap>

#include "ItemHeader.h"

class RotateLine : public QGraphicsObject
{
    Q_OBJECT
public:
    RotateLine(QGraphicsItem * parent = 0);
    ~RotateLine();

    void setMoveable(bool moveable);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void rotateItem(MouseType,int);

private:
    QGraphicsItem * parentItem;
    QRectF boundRect;

    QPointF startPoint;

    int rotateDegree;
    bool hasSelected;
    bool isMoveable;

    QPixmap pixmap;
};

#endif // ROTATELINE_H
