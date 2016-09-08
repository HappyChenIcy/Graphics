#include "mytextitem.h"

#include <QFocusEvent>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

MyTextItem::MyTextItem(GraphicsType itemType,QMenu * menu,QGraphicsItem *parent, QGraphicsScene *scene):
    menu(menu),
    type(itemType),
    QGraphicsTextItem(parent,scene)
{
    property.isFont = true;
    property.itemFont = QFont("����",15);
    setFont(property.itemFont);

    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void MyTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit textLostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void MyTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MyTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    menu->exec(event->screenPos());
}

void MyTextItem::setProperty(ItemProperty property)
{
    this->property = property;

    setFont(property.itemFont);

    setRotation(property.rotateDegree);
}

//��item�и���������ʽ
void MyTextItem::updateFont(QFont font)
{
    property.itemFont = font;
    setFont(property.itemFont);
}

//������ʾ����������̬������
QRectF MyTextItem::getBoundRect()
{
    QFontMetricsF metrics = property.itemFont;
    return metrics.boundingRect(toPlainText());
}

//QVariant MyTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
//{
//    return QGraphicsTextItem::itemChange(change,value);
//}

MyTextItem::~MyTextItem()
{

}
