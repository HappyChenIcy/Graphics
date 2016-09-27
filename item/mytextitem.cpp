#include "mytextitem.h"

#include <QFocusEvent>
#include <QPainter>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QDebug>

QDataStream & operator <<(QDataStream &stream,MyTextItem * item)
{
    int type = item->type;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyTextItem * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->type = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyTextItem::MyTextItem(GraphicsType itemType,QMenu * menu,QGraphicsItem *parent, QGraphicsScene *scene):
    menu(menu),
    type(itemType),
    QGraphicsTextItem(parent,scene)
{
    property.isFont = true;
    property.itemFont = QFont("����",FONT_DEAFULT_PIX);
    property.content = "��������...";
    setFont(property.itemFont);

    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    pixmap.load(":/images/itemLock.png");

    QTextCursor cursor = textCursor();

    cursor.insertText(property.content);
    //�ӵ�ǰλ��һ��һ��[KeepAnchor]���ƶ�����ʼλ��[Start]
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void MyTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    property.content = toPlainText();
    emit textLostFocus(this);
    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = getBoundRect().width();
    property.itemRect.height = getBoundRect().height();
    QGraphicsTextItem::focusOutEvent(event);
}

void MyTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = getBoundRect().width();
    property.itemRect.height = getBoundRect().height();
    emit posHasChanged(property.itemRect);
    QGraphicsTextItem::mouseMoveEvent(event);
}

void MyTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MyTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(menu)
    {
        setSelected(true);
        menu->exec(event->screenPos());
    }
}

void MyTextItem::setProperty(ItemProperty property)
{
    this->property = property;

    setDefaultTextColor(property.fontColor);
    setFont(property.itemFont);
    setZValue(property.zValue);

    setRotation(property.rotateDegree);
}

void MyTextItem::setZValue(qreal z)
{
    property.zValue = z;
    QGraphicsTextItem::setZValue(z);
}

//��д�˷��������ڿ���ʱ��λ����Ϣ����
void MyTextItem::setPos(const QPointF &pos)
{
    property.itemRect.x = pos.x();
    property.itemRect.y = pos.y();

    QGraphicsTextItem::setPos(pos);
}

void MyTextItem::setPos(qreal x, qreal y)
{
    property.itemRect.x = x;
    property.itemRect.y = y;

    QGraphicsTextItem::setPos(x,y);
}

//��Text�����������ڴ˵���
void MyTextItem::setCentralPos(QPointF pos)
{
    qreal realx = pos.x() - getBoundRect().width()/2;
    qreal realy = pos.y() - getBoundRect().height()/2;

    setPos(realx,realy);
}

//���������Ƿ�����϶�
void MyTextItem::setMoveable(bool lockState)
{
    this->property.isMoveable = lockState;

    setFlag(QGraphicsItem::ItemIsMovable,lockState);
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
    //��ȡ�ַ����Ŀ��
    QFontMetricsF metrics(property.itemFont);

    QStringList source = toPlainText().split("\n");

    qreal totalHeight = 0;
    qreal maxWidth = -1;

    foreach (QString s, source)
    {
        int tmpWidth = metrics.width(s);
        if(tmpWidth > maxWidth)
        {
            maxWidth = tmpWidth;
        }
        totalHeight += metrics.height();
    }

    QRectF rectF;

    rectF.setWidth(maxWidth);
    rectF.setHeight(totalHeight);

    return rectF;
}

void MyTextItem::keyPressEvent(QKeyEvent *event)
{
//    emit updateTextGeometry();
    QGraphicsTextItem::keyPressEvent(event);
}

//����Ϊ�����ؼ���һ����ʱ��������ʾ������Ϣ
void MyTextItem::cleartText()
{
    property.content = "";
    setPlainText(property.content);
}

QVariant MyTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsTextItem::itemChange(change,value);
}

MyTextItem::~MyTextItem()
{

}
