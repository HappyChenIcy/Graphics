/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myarrow.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ����ͷ����
**Others: 1��֧����ʽ������
**        2��֧���������ؼ�������
**
**�޸���ʷ:
**20160907:wey:��Ӽ�ͷ�Ļ���
**20160908:wey:֧�ֿؼ���ת�󣬼�ͷ��Ȼ��ؼ�������
**20160920:wey:��Ӷ˿ڼ�ͷ����
**20160927:wey:���������Ϣ
**20160928:wey:��ӶԼ�ͷ���͵��л�
**20150929:wey:�޸����ſؼ�ʱ���¼�ͷ���Ȳ�������
**             �޸���ͷ������ȷ����
**             �����������뿪�ı���ʽ������״̬��ʾ
**20161008:wey:���������߻�����ʼ�����ؼ�һ����е�
**20161017:gx:�޸�������ѡ������С������(Ŀǰ�ڴ�ֱ��ˮƽ״̬��ѡ��Ч����Ȼ����)
**20161110:wey:����Ҽ��˵�����
**20161209:wey:�޸���paint��ʵʱ���㵱ǰλ����Ϣ�������Ч��
*************************************************/
#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>
#include <QDataStream>

#include "../Header.h"

#include <QObject>

class MyNodePort;
class MyTextItem;
class MyNodeLine;

class MyArrow :public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    MyArrow(QGraphicsItem * parent = 0);

    ~MyArrow();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updatePosition();

    void setStartItem(MyNodeLine * startItem);
    MyNodeLine * getStartItem() const{ return startItem; }

    void setEndItem(MyNodeLine * endItem);
    MyNodeLine * getEndItem() const{ return endItem; }

    void setStartPointType(PointType type);
    PointType getStartPointType(){return this->property.startPointType;}

    void setEndPointType(PointType type);
    PointType getEndPointType(){return this->property.endPointType;}

    void setStartItemID(const QString id);
    void setEndItemID(const QString id);

    //��ȡ��ʼ�ͽ�����������ͷ������(ֱ�ߡ���ͷ��ʵ�ļ�ͷ)
    AddLineType getStartLineType(){return this->property.startLineType;}
    void setStartLineType(int type);

    AddLineType getEndLineType(){return this->property.endLineType;}
    void setEndLineType(int type);

    void setLineType(LineType lineType);
    LineType getLineType()const {return this->property.lineType;}

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    QString getText();
    void setText(QString text);

    friend QDataStream & operator <<(QDataStream &,MyArrow * item);
    friend QDataStream & operator >>(QDataStream &,MyArrow * item);

signals:
    void editMe();
    void updateSceneDraw();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void countNodeCrossPoint(QPolygonF polygon,QLineF centerLine,QPointF &endIntersectPoint);
    void countItemCrossPoint(bool isStart, QPolygonF polygon, QLineF centerLine, QPointF &endIntersectPoint);
    void countArrowPoint(bool isStart, QPointF startPoint, QPointF &point1, QPointF &point2);
    QPolygonF countArrowPolygon(QPointF startPoint, bool isStart);

    void createTextItem();

    MyNodeLine * startItem;
    MyNodeLine * endItem;

    QPolygonF arrowHead;

    GraphicsType type;
    ItemProperty property;

    QRectF boundRect;

    MyTextItem * myTextItem;           //������Ϣ
};

#endif // MYARROW_H
