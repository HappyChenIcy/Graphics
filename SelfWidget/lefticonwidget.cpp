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
    polygonWidget->addItem(square);

    MyListItem * rect = ListItemManager::instance()->createListItem(Constants::RECT_ID,QIcon(":/images/rectange.png"),"����",polygonWidget);
    polygonWidget->addItem(rect);

    MyListItem * roundRect = ListItemManager::instance()->createListItem(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"Բ�Ǿ���",polygonWidget);
    polygonWidget->addItem(roundRect);

    MyListItem * circle = ListItemManager::instance()->createListItem(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"Բ",polygonWidget);
    polygonWidget->addItem(circle);

    MyListItem * ellipse = ListItemManager::instance()->createListItem(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"��Բ",polygonWidget);
    polygonWidget->addItem(ellipse);

    MyListItem * polygon = ListItemManager::instance()->createListItem(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"����",polygonWidget);
    polygonWidget->addItem(polygon);

    //����ڵ�
    MyListItem * line = ListItemManager::instance()->createListItem(Constants::LINE_ID,QIcon(":/images/linepointer.png"),"����",nodeWidget);
    nodeWidget->addItem(line);

    MyListItem * vectorLine = ListItemManager::instance()->createListItem(Constants::VECTOR_LINE_ID,QIcon(":/images/vectorLine.png"),"������",nodeWidget);
    nodeWidget->addItem(vectorLine);

    MyListItem * nodePort = ListItemManager::instance()->createListItem(Constants::NODE_PORT_ID,QIcon(":/images/nodePort.png"),"�˿�",nodeWidget);
    nodeWidget->addItem(nodePort);

}

LeftIconWidget::~LeftIconWidget()
{
    delete toolBox;
    delete polygonWidget;
    delete nodeWidget;
}
