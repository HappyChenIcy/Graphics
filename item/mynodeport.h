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
**20160930:wey:��������QGraphicsPolygonItem
**20161027:wey:�������Ǻ�D�˿�
**20161028:wey:����������Ϣ��ʾ
**             �����Ҽ��˵���ȫ��ͬһ����
*************************************************/
#ifndef MYNODEPORT_H
#define MYNODEPORT_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QBrush>

#include "../Header.h"
#include "mynodeline.h"

#include <QDataStream>

class QMenu;
class MyItem;
class MyArrow;
class MyTextItem;

class MyNodePort : public MyNodeLine
{
    Q_OBJECT
public:
    MyNodePort(GraphicsType type, MyItem * parentItem, QObject * parent1 = 0);
    ~MyNodePort();

    QRectF boundingRect() const;

    void setPos(const QPointF &pos);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDragDirect(DragDirect direct);
    DragDirect getDragDirect(){return this->nodeProperty.direct;}

    void setNextDirect(DragDirect direct);
    DragDirect getNextDirect(){return this->nextDirect;}

    bool isArrivalLimitRang(){return this->arrivalLimitRang;}
    void setArrivalLimitRang(bool flag);

    void setScaleFactor(qreal scaleFactor);
    qreal getScaleFactor(){return this->nodeProperty.scaleFactor;}

    void setProperty(ItemProperty property);

    void setNodeProperty(NodePortProperty prop);
    NodePortProperty  getNodeProperty(){return this->nodeProperty;}

    QPolygonF getScenePolygon();

    void updatePortID(QString portId);

    void setMoveable(bool isMoveable);

    friend QDataStream & operator >>(QDataStream & ,MyNodePort *);
    friend QDataStream & operator <<(QDataStream & ,MyNodePort *);

    QString getText();
    void setText(QString text);

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
    void updateTextPosByDirect();

private:
    void initWidget(MyItem *parentItem);
    void initNodePortRightMenu();
    bool isMoveable;                   //�Ƿ�֧���ƶ������ڸ���������

    NodePortProperty nodeProperty;     //�˿�����
    DragDirect nextDirect;             //�˿ڵ���յ�ʱ���˱����ᱻ��ֵ���������һʱ���ƶ�ʱ��ο���ǰ�����Լ��¸�����
    bool arrivalLimitRang;             //�˿��Ƿ񵽴��߶εĶ˵�

    QMenu * nodePortRightMenu;         //�˿��Ҽ��˵���֧��ɾ��
    MyTextItem * myTextItem;           //������Ϣ

    QList<MyArrow *> arrows;           //������ӵļ�ͷ
};

#endif // MYNODEPORT_H
