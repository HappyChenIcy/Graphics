#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "global.h"
#include "myitem.h"
#include "myarrow.h"

MyScene::MyScene(QMenu *menu, QObject * parent):
    rightMenu(menu),
    QGraphicsScene(parent)
{
    CurrAddGraType =  GRA_NONE;
    insertTmpLine = NULL;

}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button () == Qt::LeftButton)
    {
        if(CurrAddGraType == GRA_LINE)
        {
            insertTmpLine = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
            insertTmpLine->setPen(QPen(Qt::red, 2));
            addItem(insertTmpLine);
        }
        else
        {
            switch(CurrAddGraType)
            {
                 case GRA_SQUARE:
                                    MyItem * myItem = new MyItem(rightMenu);
                                    myItem->setPos(event->scenePos());
                                    connect(myItem,SIGNAL(updateSceneDraw()),this,SLOT(update()));
                                    addItem(myItem);
                                 break;
            }

        }
    }

    if(CurrAddGraType != GRA_NONE && CurrAddGraType != GRA_LINE)
    {
        CurrAddGraType = GRA_NONE;
        emit resetItemAction();
    }

    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(CurrAddGraType == GRA_LINE && insertTmpLine)
    {
        QLineF newLine(insertTmpLine->line().p1(), event->scenePos());
        insertTmpLine->setLine(newLine);
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(CurrAddGraType == GRA_LINE && insertTmpLine)
    {
        QList<QGraphicsItem *> startItems = items(insertTmpLine->line().p1());
        if (startItems.count() && startItems.first() == insertTmpLine)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items(insertTmpLine->line().p2());
        if (endItems.count() && endItems.first() == insertTmpLine)
        {
            endItems.removeFirst();
        }

        removeItem(insertTmpLine);
        delete insertTmpLine;

        if (startItems.count() > 0 && endItems.count() > 0 &&
//            startItems.first()->type() == DiagramItem::Type &&
//            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first())
        {
            MyItem *startItem = qgraphicsitem_cast<MyItem *>(startItems.first());
            MyItem *endItem = qgraphicsitem_cast<MyItem *>(endItems.first());
            MyArrow *arrow = new MyArrow(startItem, endItem);
//            arrow->setColor(Qt::red);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
    insertTmpLine = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

MyScene::~MyScene()
{

}
