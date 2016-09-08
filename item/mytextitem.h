/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mytextitem.h
**��  ��: wey       Version: 1.0       Date: 2016.09.06
**��  ��: �Զ������ֿؼ�
**Others: 1��֧�����ֵ�˫���༭
**        2��֧����ʽ���趨������������ĸ�ʽ
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QObject>

class QMenu;

#include "../Header.h"

class MyTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyTextItem(GraphicsType itemType,QMenu * menu,QGraphicsItem *parent2 = 0, QGraphicsScene *scene = 0);
    ~MyTextItem();

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    void updateFont(QFont font);

    GraphicsType getType(){return this->type;}

    int getWidth();

//    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void textLostFocus(MyTextItem *item);

protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    GraphicsType type;

    ItemProperty property;

    QMenu * menu;
};

#endif // MYTEXTITEM_H
