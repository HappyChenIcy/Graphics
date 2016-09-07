#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QGraphicsView>
#include <QToolBar>
#include <QActionGroup>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDebug>

#include "actionmanager.h"
#include "item/myscene.h"
#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/righttoolbox.h"
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

    showMaximized();
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

    MyAction * rotateLeftAction = ActionManager::instance()->crateAction(Constants::ROTATE_LEFT_ID,QIcon(":/images/rotateLeft.png"),"��ת90��");
    ActionManager::instance()->registerAction(rotateLeftAction,this,SLOT(rotateItem()));

    MyAction * rotateRightAction = ActionManager::instance()->crateAction(Constants::ROTATE_RIGHT_ID,QIcon(":/images/rotateRight.png"),"��ת90��");
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
    arrowAction->setChecked(true);

    MyAction * squareAction = ActionManager::instance()->crateAction(Constants::SQUARE_ID,QIcon(":/images/square.png"),"������");
    ActionManager::instance()->registerAction(squareAction,this,SLOT(addItem()),true);
    squareAction->setType(GRA_SQUARE);

    MyAction * rectAction = ActionManager::instance()->crateAction(Constants::RECT_ID,QIcon(":/images/rectange.png"),"����");
    ActionManager::instance()->registerAction(rectAction,this,SLOT(addItem()),true);
    rectAction->setType(GRA_RECT);

    MyAction * roundRectAction = ActionManager::instance()->crateAction(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"Բ�Ǿ���");
    ActionManager::instance()->registerAction(roundRectAction,this,SLOT(addItem()),true);
    roundRectAction->setType(GRA_ROUND_RECT);

    MyAction * circleAction = ActionManager::instance()->crateAction(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"Բ");
    ActionManager::instance()->registerAction(circleAction,this,SLOT(addItem()),true);
    circleAction->setType(GRA_CIRCLE);

    MyAction * ellipseAction = ActionManager::instance()->crateAction(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"��Բ");
    ActionManager::instance()->registerAction(ellipseAction,this,SLOT(addItem()),true);
    ellipseAction->setType(GRA_ELLIPSE);

    MyAction * polygonAction = ActionManager::instance()->crateAction(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"����");
    ActionManager::instance()->registerAction(polygonAction,this,SLOT(addItem()),true);
    polygonAction->setType(GRA_POLYGON);

    MyAction * lineAction = ActionManager::instance()->crateAction(Constants::LINE_ID,QIcon(":/images/linepointer.png"),"����");
    ActionManager::instance()->registerAction(lineAction,this,SLOT(addItem()),true);
    lineAction->setType(GRA_LINE);

    MyAction * textAction = ActionManager::instance()->crateAction(Constants::TEXT_ID,QIcon(":/images/text.png"),"����");
    ActionManager::instance()->registerAction(textAction,this,SLOT(addItem()),true);
    textAction->setType(GRA_TEXT);

    //���ڿ���һ����ֻ��һ��״̬��ѡ��
    itemGroup = new QActionGroup(this);

    itemGroup->addAction(arrowAction);
    itemGroup->addAction(squareAction);
    itemGroup->addAction(rectAction);
    itemGroup->addAction(roundRectAction);
    itemGroup->addAction(circleAction);
    itemGroup->addAction(ellipseAction);
    itemGroup->addAction(polygonAction);
    itemGroup->addAction(lineAction);
    itemGroup->addAction(textAction);

    itemMenu->addAction(arrowAction);
    itemMenu->addAction(squareAction);
    itemMenu->addAction(rectAction);
    itemMenu->addAction(roundRectAction);
    itemMenu->addAction(circleAction);
    itemMenu->addAction(ellipseAction);
    itemMenu->addAction(polygonAction);
    itemMenu->addAction(lineAction);
    itemMenu->addAction(textAction);
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
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    if(selectedItems.size() !=  1)
    {
        return;
    }

    MyItem * graphicsTmp = dynamic_cast<MyItem *>(scene->selectedItems().first());

    QString objName = QObject::sender()->objectName();
    if(objName == QString(Constants::ROTATE_LEFT_ID))
    {
        graphicsTmp->setRotation(-90);
        graphicsTmp->updateRotation(-90);
    }
    else if(objName == QString(Constants::ROTATE_RIGHT_ID))
    {
        graphicsTmp->setRotation(90);
        graphicsTmp->updateRotation(90);
    }
}

void MainWindow::bringZItem()
{
    if (scene->selectedItems().isEmpty())
    {
         return;
    }

    QGraphicsItem *selectedItem = scene->selectedItems().first();
//    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();     //�ҵ��뵱ǰitem���غϵ�items
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

//����Ҫ��scene��ɾ��item
void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    foreach(QGraphicsItem * item, selectedItems)
    {
        MyArrow * tmp = dynamic_cast<MyArrow *>(item);
        if(tmp)
        {
            tmp->getStartItem()->removeArrow(tmp);
            tmp->getEndItem()->removeArrow(tmp);
            delete tmp;
        }
    }

    foreach (QGraphicsItem * item, selectedItems)
    {
        MyItem * tmp = dynamic_cast<MyItem *>(item);
        if(tmp)
        {
            tmp->removeArrows();
            scene->removeItem(tmp);
            delete tmp;
        }
    }
    scene->update();
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
    QWidget * centralWidget = new QWidget;
    QHBoxLayout * layout = new QHBoxLayout;

    SceneWidth = SceneHeight = 5000;

    scene = new MyScene(rightMenu);
    scene->setSceneRect(0,0,SceneWidth,SceneHeight);

    connect(scene,SIGNAL(resetItemAction()),this,SLOT(respRestItemAction()));
    connect(scene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));

    view = new QGraphicsView(this);
    view->setScene(scene);
//    view->setDragMode(QGraphicsView::RubberBandDrag);   //������Ӻ�ᵼ��scene�޷�����mousemove�¼�
//    view->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);

    rightToolBox = new RightToolBox;
    connect(this,SIGNAL(initToolBox(int,ItemProperty)),rightToolBox,SLOT(respInitToolBox(int,ItemProperty)));
    connect(rightToolBox,SIGNAL(updateProperty(ItemProperty)),this,SLOT(respPropertyUpdate(ItemProperty)));
    connect(rightToolBox,SIGNAL(deleteCurrItem()),this,SLOT(deleteItem()));

    layout->addWidget(view);
    layout->addWidget(rightToolBox);
    centralWidget->setLayout(layout);

    this->setCentralWidget(centralWidget);
}

//����scene���һ�ʱ����item�������е�״̬�ָ�����ͷ״̬
void MainWindow::respRestItemAction()
{
    ActionManager::instance()->action(Constants::ARROW_ID)->setChecked(true);
}

//��ѡ���item״̬�ı�󣬸���action
void MainWindow::updateActions()
{
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    int selectedSize = scene->selectedItems().size();

    ItemProperty  property;

    if(selectedSize == 0)
    {
        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(false);
    }
    else if(selectedSize == 1)
    {
        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        MyItem * myItem = dynamic_cast<MyItem *>(scene->selectedItems().first());

        if(myItem)
        {
            property = myItem->getProperty();
        }
        //Ϊtextģʽ
        else
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(scene->selectedItems().first());
            if(textItem)
            {
                property = textItem->getProperty();
            }
        }
        //        qDebug()<< myItem->pos().x()<<"==="<<myItem->pos().y();
//        qDebug()<< myItem->scenePos().x()<<"========="<<myItem->scenePos().y();
//        qDebug()<< myItem->sceneBoundingRect().x()<<"==========="<<myItem->sceneBoundingRect().y();
//        qDebug()<< myItem->boundingRect().width()<<"===%%%%==="<<myItem->boundingRect().height();
//        qDebug()<< myItem->getProperty().itemRect.width<<"=="<<myItem->getProperty().itemRect.height;
    }
    else
    {
        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);
    }

    emit initToolBox(selectedSize,property);
}

//����item������
void MainWindow::respPropertyUpdate(ItemProperty property)
{
    if(scene->selectedItems().size() == 1)
    {
        MyItem * myItem = dynamic_cast<MyItem *>(scene->selectedItems().first());
        if(myItem)
        {
            myItem->setProperty(property);
        }
        else
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(scene->selectedItems().first());
            if(textItem)
            {
                textItem->setProperty(property);
            }
        }
    }
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
    QToolBar * itemBar = addToolBar("Item");
    itemBar->addAction(ActionManager::instance()->action(Constants::ARROW_ID));
    itemBar->addSeparator();
    itemBar->addAction(ActionManager::instance()->action(Constants::SQUARE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::RECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::ROUNDRECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::CIRCLE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::ELLIPSE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::POLYGON_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::TEXT_ID));

    QToolBar * editBar = addToolBar("Edit");
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    QToolBar * sceneBar = addToolBar("Scene");

//    addToolBar(Qt::BottomToolBarArea,sceneBar);

//    sizeBox = new QComboBox;
//    QStringList list;
//    list<<"50%"<<"75%"<<"100%"<<"125%"<<"150%";
//    sizeBox->addItems(list);
//    sizeBox->setCurrentIndex(2);
//    connect(sizeBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(sceneScaled(QString)));
//    sceneBar->addWidget(sizeBox);

    mySlider = new MySlider;
    connect(mySlider,SIGNAL(scaleView(int)),this,SLOT(sceneScaled(int)));
    sceneBar->addWidget(mySlider);
}

//�л���ͼ����
void MainWindow::sceneScaled(int currScale)
{
//     double newScale = currScale.left(currScale.indexOf(tr("%"))).toDouble() / 100.0;
     double newScale = currScale/100.0;
     QMatrix oldMatrix = view->matrix();
     view->resetMatrix();
     view->translate(oldMatrix.dx(), oldMatrix.dy());
     view->scale(newScale, newScale);
}

MainWindow::~MainWindow()
{
    delete mySlider;

    delete ui;
}
