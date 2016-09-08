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

signals:
    void updateSceneDraw();
    void posHasChanged(MyRect);

private slots:
    void procDragSize(PointType type);
    void procResizeItem();
    void procRotate(int degree);
    void procMouseState(MouseType);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void setDragPointVisible(bool flag);
    void updateRotateLinePos();

    int radius;

    GraphicsType currItemType;

    QRectF boundRect;

    QPen selectedPen;
    int selectedPenWidth;

    DragPoint * leftTopPoint;
    DragPoint * rightTopPoint;
    DragPoint * leftBottomPoint;
    DragPoint * rightBottomPoint;

    MyTextItem * myTextItem;           //������Ϣ

    RotateLine * rotateLine;
    QGraphicsScene * parentScene;      //���ڵ�����

    QMenu * rightMenu;

    MouseType currMouseType;

    QList<MyArrow *> arrows;           //������ӵļ�ͷ
    QPolygonF itemPolygon;             //��ǰͼ�εĸ����������꼯��

    ItemProperty property;             //���浱ǰ����

    bool isNeedBorder;                 //�Ƿ���Ҫѡ�б߿�
};

#endif // MYITEM_H
