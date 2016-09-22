#include "lefticonwidget.h"

#include <QToolBox>
#include <QHBoxLayout>
#include <QDebug>

#include "listitemmanager.h"
#include "mylistwidget.h"
#include "listitemmanager.h"
#include "../Constants.h"

using namespace Graphics;

LeftIconWidget::LeftIconWidget(QWidget *parent):
    QWidget(parent)
{
    setFixedWidth(150);

    initWidget();
}

//��ʼ���ؼ�
void LeftIconWidget::initWidget()
{
    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(1,1,1,1);

    toolBox = new QToolBox(this);

    polygonWidget = new MyListWidget;

    nodeWidget = new MyListWidget;

    initListItems();

    toolBox->addItem(polygonWidget,"Polygon");
    toolBox->addItem(nodeWidget,"Nodes");

    mainLayout->addWidget(toolBox);

    this->setLayout(mainLayout);
}

//��ʼ���б�item
void LeftIconWidget::initListItems()
{
    //��������
    MyListItem * square = ListItemManager::instance()->createListItem(Constants::SQUARE_ID,QIcon(":/images/square.png"),"������",polygonWidget);
    square->setData(Qt::UserRole,(int)GRA_SQUARE);
    polygonWidget->addItem(square);

    MyListItem * rect = ListItemManager::instance()->createListItem(Constants::RECT_ID,QIcon(":/images/rectange.png"),"����",polygonWidget);
    rect->setData(Qt::UserRole,(int)GRA_RECT);
    polygonWidget->addItem(rect);

    MyListItem * roundRect = ListItemManager::instance()->createListItem(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"Բ�Ǿ���",polygonWidget);
    roundRect->setData(Qt::UserRole,(int)GRA_ROUND_RECT);
    polygonWidget->addItem(roundRect);

    MyListItem * circle = ListItemManager::instance()->createListItem(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"Բ",polygonWidget);
    circle->setData(Qt::UserRole,(int)GRA_CIRCLE);
    polygonWidget->addItem(circle);

    MyListItem * ellipse = ListItemManager::instance()->createListItem(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"��Բ",polygonWidget);
    ellipse->setData(Qt::UserRole,(int)GRA_ELLIPSE);
    polygonWidget->addItem(ellipse);

    MyListItem * polygon = ListItemManager::instance()->createListItem(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"����",polygonWidget);
    polygon->setData(Qt::UserRole,(int)GRA_POLYGON);
    polygonWidget->addItem(polygon);

    MyListItem * text = ListItemManager::instance()->createListItem(Constants::TEXT_ID,QIcon(":/images/text.png"),"����",polygonWidget);
    text->setData(Qt::UserRole,(int)GRA_TEXT);
    polygonWidget->addItem(text);

    //����ڵ�
    MyListItem * nodePort = ListItemManager::instance()->createListItem(Constants::NODE_PORT_ID,QIcon(":/images/nodePort.png"),"�˿�",nodeWidget);
    nodePort->setData(Qt::UserRole,(int)GRA_NODE_PORT);
    nodeWidget->addItem(nodePort);
}

LeftIconWidget::~LeftIconWidget()
{
    if(polygonWidget)
    {
        delete polygonWidget;
        polygonWidget = NULL;
    }

    if(nodeWidget)
    {
        delete nodeWidget;
        nodeWidget = NULL;
    }

    if(toolBox)
    {
        delete toolBox;
        toolBox = NULL;
    }
}
