#include "lefticonwidget.h"

#include <QToolBox>
#include <QHBoxLayout>
#include <QDebug>

#include "listitemmanager.h"
#include "mylistwidget.h"

LeftIconWidget::LeftIconWidget(QWidget *parent):
    QWidget(parent)
{
    setFixedWidth(120);


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
    QListWidgetItem * square = new QListWidgetItem(QIcon(":/images/square.png"),"������",polygonWidget);
    square->setSizeHint(QSize(60,60));

    square->setData(Qt::UserRole,"MyListItem");
    polygonWidget->addItem(square);

    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/rectange.png"),"����",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/roundedrect.png"),"Բ�Ǿ���",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/circle.png"),"Բ",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/ellipse.png"),"��Բ",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/diamonds.png"),"����",polygonWidget));

    //����ڵ�
    nodeWidget->addItem(new QListWidgetItem(QIcon(":/images/linepointer.png"),"����",nodeWidget));
    nodeWidget->addItem(new QListWidgetItem(QIcon(":/images/vectorLine.png"),"������",nodeWidget));
}

LeftIconWidget::~LeftIconWidget()
{
    delete toolBox;
    delete polygonWidget;
    delete nodeWidget;
}
