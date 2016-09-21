/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mynodeport.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��: �ؼ��ϵĶ˿�
**Others:1.��Ҫ֧������
**       2.֧���ƶ�����̬���
**       3.֧������
**
**�޸���ʷ:
**20160919:wey:����Ҽ�ɾ���¼�
**             ����Ҽ��༭����
**             �������߹���
**20160920:wey:���ӱ��ر���
*************************************************/
#ifndef MYNODEPORT_H
#define MYNODEPORT_H

#include <QGraphicsObject>
#include <QBrush>

#include "ItemHeader.h"
#include "../Header.h"

#include <QDataStream>

class QMenu;
class MyItem;
class MyArrow;

class MyNodePort : public QGraphicsObject
{
    Q_OBJECT
public:
    MyNodePort(MyItem * parentItem);
    ~MyNodePort();

    QRectF boundingRect() const;

    void setPos(const QPointF &pos);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDragDirect(DragDirect direct);
    DragDirect getDragDirect(){return this->nodeProperty.direct;}

    void setScaleFactor(qreal scaleFactor);
    qreal getScaleFactor(){return this->nodeProperty.scaleFactor;}

    void setProperty(ItemProperty property);

    GraphicsType getType()const{return this->type;}

    void setNodeProperty(NodePortProperty prop);
    NodePortProperty  getNodeProperty(){return this->nodeProperty;}

    QPolygonF getScenePolygon();

    MyItem * getParentItem(){return this->parentMyItem;}

    friend QDataStream & operator >>(QDataStream & ,MyNodePort *);
    friend QDataStream & operator <<(QDataStream & ,MyNodePort *);

    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

signals:
    void deletePort(MyNodePort *);
    void editPort(MyNodePort *);
    void portPosChanged(MouseType,QPointF);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private slots:
    void respDeleteAction();
    void respEditAction();

private:
    void initNodePortRightMenu();

    QRectF boundRect;
    int radius;

    GraphicsType  type;                //�ڵ�����

    NodePortProperty nodeProperty;     //�˿�����

    QMenu * nodePortRightMenu;         //�˿��Ҽ��˵���֧��ɾ��

    QList<MyArrow *> arrows;           //������ӵļ�ͷ

    MyItem * parentMyItem;             //���ڵ�
};

#endif // MYNODEPORT_H
