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
*************************************************/
#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>
#include <QDataStream>

#include "../Header.h"
#include "ItemHeader.h"

#include <QObject>

class MyItem;
class MyNodePort;
class MyTextItem;

class MyArrow :public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem * parent = 0);
    MyArrow(MyNodePort  * startItem,MyNodePort  * endItem,QGraphicsItem * parent = 0);

    ~MyArrow();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updatePosition();

    MyItem * getStartItem() const{ return startItem; }
    MyItem * getEndItem() const{ return endItem; }

    MyNodePort * getStartNodePort() const{return this->startNodePort;}
    MyNodePort * getEndNodePort() const{return this->endNodePort;}

    LineType getLineType()const {return this->property.lineType;}

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    QString getText();
    void setText(QString text);

    friend QDataStream & operator <<(QDataStream &,MyArrow * item);
    friend QDataStream & operator >>(QDataStream &,MyArrow * item);

signals:
    void editMe();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    void createTextItem();

    MyItem  * startItem;
    MyItem  * endItem;

    MyNodePort * startNodePort;
    MyNodePort * endNodePort;

    QPolygonF arrowHead;

    GraphicsType type;
    ItemProperty property;

    QRectF boundRect;

    MyTextItem * myTextItem;           //������Ϣ
};

#endif // MYARROW_H
