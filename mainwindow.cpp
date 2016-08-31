#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QGraphicsView>
#include <QToolBar>
#include <QActionGroup>
#include <QDebug>

#include "actionmanager.h"
#include "item/myscene.h"
#include "./item/myitem.h"
#include "global.h"

using namespace Graphics;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rightMenu = NULL;

    createActionAndMenus();

    createContextMenu();

    createSceneAndView();

    createToolBar();
}

//�������ڵĲ˵���������Ӧ�¼�
void MainWindow::createActionAndMenus()
{
    QMenu * fileMenu = menuBar()->addMenu("�ļ�(&F)");

    MyAction * fileAction = ActionManager::instance()->crateAction(Constants::FILE_ID,tr("File"));
    ActionManager::instance()->registerAction(fileAction,this,SLOT(fileOpen()));

    MyAction * exitAction = ActionManager::instance()->crateAction(Constants::EXIT_ID,tr("Exit"));
    ActionManager::instance()->registerAction(exitAction,this,SLOT(exitApp()));

    fileMenu->addAction(fileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu * editMenu = menuBar()->addMenu("�༭(&E)");

    MyAction * rotateLeftAction = ActionManager::instance()->crateAction(Constants::ROTATE_LEFT_ID,QIcon(":/images/rotateLeft.png"),"������ת");
    ActionManager::instance()->registerAction(rotateLeftAction,this,SLOT(rotateItem()));

    MyAction * rotateRightAction = ActionManager::instance()->crateAction(Constants::ROTATE_RIGHT_ID,QIcon(":/images/rotateRight.png"),"������ת");
    ActionManager::instance()->registerAction(rotateRightAction,this,SLOT(rotateItem()));

    MyAction * bringFrontAction = ActionManager::instance()->crateAction(Constants::BRING_FRONT_ID,QIcon(":/images/bringtofront.png"),"���ڶ���");
    ActionManager::instance()->registerAction(bringFrontAction,this,SLOT(bringZItem()));

    MyAction * bringBackAction = ActionManager::instance()->crateAction(Constants::BRING_BACK_ID,QIcon(":/images/sendtoback.png"),"���ڵײ�");
    ActionManager::instance()->registerAction(bringBackAction,this,SLOT(bringZItem()));

    MyAction * deleteAction = ActionManager::instance()->crateAction(Constants::DELETE_ID,QIcon(":/images/delete.png"),"ɾ��");
    ActionManager::instance()->registerAction(deleteAction,this,SLOT(deleteItem()));

    editMenu->addAction(rotateLeftAction);
    editMenu->addAction(rotateRightAction);
    editMenu->addAction(bringFrontAction);
    editMenu->addAction(bringBackAction);
    editMenu->addAction(deleteAction);

    QMenu * itemMenu = menuBar()->addMenu("��Ŀ(&I)");

    MyAction * arrowAction = ActionManager::instance()->crateAction(Constants::ARROW_ID,QIcon(":/images/pointer.png"),"��ͷ");
    ActionManager::instance()->registerAction(arrowAction,this,SLOT(addItem()),true);
    arrowAction->setType(GRA_NONE);

    MyAction * squareAction = ActionManager::instance()->crateAction(Constants::SQUARE_ID,QIcon(":/images/square.png"),"������");
    ActionManager::instance()->registerAction(squareAction,this,SLOT(addItem()),true);
    squareAction->setType(GRA_SQUARE);

    MyAction * rectAction = ActionManager::instance()->crateAction(Constants::RECT_ID,"����");
    ActionManager::instance()->registerAction(rectAction,this,SLOT(addItem()),true);
    rectAction->setType(GRA_RECT);

    MyAction * circleAction = ActionManager::instance()->crateAction(Constants::CIRCLE_ID,"Բ��");
    ActionManager::instance()->registerAction(circleAction,this,SLOT(addItem()),true);
    circleAction->setType(GRA_CIRCLE);

    MyAction * polygonAction = ActionManager::instance()->crateAction(Constants::POLYGON_ID,"����");
    ActionManager::instance()->registerAction(polygonAction,this,SLOT(addItem()),true);
    polygonAction->setType(GRA_POLYGON);

    MyAction * lineAction = ActionManager::instance()->crateAction(Constants::LINE_ID,QIcon(":/images/linepointer.png"),"����");
    ActionManager::instance()->registerAction(lineAction,this,SLOT(addItem()),true);
    lineAction->setType(GRA_LINE);

    //���ڿ���һ����ֻ��һ��״̬��ѡ��
    itemGroup = new QActionGroup(this);

    itemGroup->addAction(arrowAction);
    itemGroup->addAction(squareAction);
    itemGroup->addAction(rectAction);
    itemGroup->addAction(circleAction);
    itemGroup->addAction(polygonAction);
    itemGroup->addAction(lineAction);

    itemMenu->addAction(arrowAction);
    itemMenu->addAction(squareAction);
    itemMenu->addAction(rectAction);
    itemMenu->addAction(circleAction);
    itemMenu->addAction(polygonAction);
    itemMenu->addAction(lineAction);
}

void MainWindow::fileOpen()
{
    scene->update();
}

void MainWindow::exitApp()
{
    qApp->exit();
}

void MainWindow::rotateItem()
{
    QString objName = QObject::sender()->objectName();
    if(objName == QString(Constants::ROTATE_LEFT_ID))
    {

    }
    else if(objName == QString(Constants::ROTATE_RIGHT_ID))
    {

    }
}

void MainWindow::bringZItem()
{
    if (scene->selectedItems().isEmpty())
    {
         return;
    }

    QGraphicsItem *selectedItem = scene->selectedItems().first();
//    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();
    QList<QGraphicsItem * > allItems = scene->items();

    bool isFront = false;
    QString objName = QObject::sender()->objectName();
    if(objName == QString(Constants::BRING_FRONT_ID))
    {
        isFront = true;
    }

    qreal zValue = 0;
    foreach (QGraphicsItem *item, allItems)
    {
         if(isFront)
         {
             if (item->zValue() >= zValue)
             {
                 zValue = item->zValue() + 0.1;
             }
         }
         else
         {
             if (item->zValue() <= zValue)
             {
                 zValue = item->zValue() - 0.1;
             }
         }
     }
    selectedItem->setZValue(zValue);
}

void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        MyItem * tmp = dynamic_cast<MyItem *>(item);
        if(tmp)
        {
            scene->removeItem(item);
            delete item;
        }
    }
}

void MainWindow::addItem()
{
    MyAction * tmpAction = dynamic_cast<MyAction *>(QObject::sender());
    if(tmpAction)
    {
        CurrAddGraType = tmpAction->getType();
    }
}

//������������ͼ
void MainWindow::createSceneAndView()
{
    scene = new MyScene(rightMenu);
    scene->setSceneRect(0,0,5000,5000);

    connect(scene,SIGNAL(resetItemAction()),this,SLOT(respRestItemAction()));

    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);

    this->setCentralWidget(view);
}

//����scene���һ�ʱ����item�������е�״̬�ָ�����ͷ״̬
void MainWindow::respRestItemAction()
{
    ActionManager::instance()->action(Constants::ARROW_ID)->setChecked(true);
}

//�����Ҽ��˵�����Ϊÿ������еĹ���
void MainWindow::createContextMenu()
{
    rightMenu = new QMenu;

    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

}

//����������
void MainWindow::createToolBar()
{
    QToolBar * editBar = addToolBar("Edit");
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    QToolBar * itemBar = addToolBar("Item");
    itemBar->addAction(ActionManager::instance()->action(Constants::ARROW_ID));
    itemBar->addSeparator();
    itemBar->addAction(ActionManager::instance()->action(Constants::SQUARE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::RECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::CIRCLE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::POLYGON_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
}

MainWindow::~MainWindow()
{
    delete ui;
}
