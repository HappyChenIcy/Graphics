#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QGraphicsView>
#include <QToolBar>
#include <QActionGroup>
#include <QComboBox>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>

#include "actionmanager.h"
#include "item/myscene.h"
#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/righttoolbox.h"
#include "./SelfWidget/mytextinput.h"
#include "fileoperate.h"
#include "global.h"

#include "typeinfo.h"

using namespace Graphics;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ģ�͹���");

    rightMenu = NULL;

    createActionAndMenus();

    createContextMenu();

    createSceneAndView();

    createToolBar();

    createStatusBar();

    showMaximized();

    respItemSizeChanged(0);

}

//�������ڵĲ˵���������Ӧ�¼�
void MainWindow::createActionAndMenus()
{
    QMenu * fileMenu = menuBar()->addMenu("�ļ�(&F)");

    MyAction * newAction = ActionManager::instance()->crateAction(Constants::FILE_ID,QIcon(":/images/new.png"),"�½�");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    ActionManager::instance()->registerAction(newAction,this,SLOT(fileOpen()));

    MyAction * saveAction = ActionManager::instance()->crateAction(Constants::SAVE_ID,QIcon(":/images/save.png"),"����");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    ActionManager::instance()->registerAction(saveAction,this,SLOT(fileSave()));

    MyAction * openAction = ActionManager::instance()->crateAction(Constants::OPEN_ID,QIcon(":/images/open.png"),"��");
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    ActionManager::instance()->registerAction(openAction,this,SLOT(fileOpen()));

    MyAction * clearAction = ActionManager::instance()->crateAction(Constants::CLEAR_ID,QIcon(":/images/clear.png"),"���");
    ActionManager::instance()->registerAction(clearAction,this,SLOT(fileClear()));

    MyAction * exitAction = ActionManager::instance()->crateAction(Constants::EXIT_ID,"�˳�");
    exitAction->setShortcut(QKeySequence("Ctrl+E"));
    ActionManager::instance()->registerAction(exitAction,this,SLOT(exitApp()));

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(clearAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu * editMenu = menuBar()->addMenu("�༭(&E)");

    MyAction * cutAction = ActionManager::instance()->crateAction(Constants::CUT_ID,QIcon(":/images/cut.png"),"����");
    cutAction->setShortcut(QKeySequence("Ctrl+X"));
    ActionManager::instance()->registerAction(cutAction,this,SLOT(cutItem()));

    MyAction * copyAction = ActionManager::instance()->crateAction(Constants::COPY_ID,QIcon(":/images/copy.png"),"����");
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    ActionManager::instance()->registerAction(copyAction,this,SLOT(copyItem()));

    MyAction * pasteAction = ActionManager::instance()->crateAction(Constants::PASTE_ID,QIcon(":/images/paste.png"),"ճ��");
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    ActionManager::instance()->registerAction(pasteAction,this,SLOT(pasteItem()));

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

    MyAction * editTextAction = ActionManager::instance()->crateAction(Constants::EDIT_TEXT_ID,QIcon(":/images/editText.png"),"�༭�ı�");
    editTextAction->setShortcut(QKeySequence("Ctrl+T"));
    ActionManager::instance()->registerAction(editTextAction,this,SLOT(editTextItem()));

    editMenu->addAction(editTextAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
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

    MyAction * vectorLineAction = ActionManager::instance()->crateAction(Constants::VECTOR_LINE_ID,QIcon(":/images/vectorLine.png"),"������");
    ActionManager::instance()->registerAction(vectorLineAction,this,SLOT(addItem()),true);
    vectorLineAction->setType(GRA_VECTOR_LINE);

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
    itemGroup->addAction(textAction);
    itemGroup->addAction(lineAction);
    itemGroup->addAction(vectorLineAction);

    itemMenu->addAction(arrowAction);
    itemMenu->addAction(squareAction);
    itemMenu->addAction(rectAction);
    itemMenu->addAction(roundRectAction);
    itemMenu->addAction(circleAction);
    itemMenu->addAction(ellipseAction);
    itemMenu->addAction(polygonAction);
    itemMenu->addAction(textAction);
    itemMenu->addAction(lineAction);
    itemMenu->addAction(vectorLineAction);
}

//�½��հ׿ռ�
void MainWindow::fileNew()
{

}

//�򿪱��ر�����ļ���������ʾ�Ƿ�Ҫ���浱ǰ��ӵĿؼ�
void MainWindow::fileOpen()
{
    respRestItemAction();

    if(scene->items().size()>0)
    {
        fileClear();
    }

    QString openFileName = QFileDialog::getOpenFileName(this,"ѡ����ļ�","","Files(*"+SaveFileSuffix+")");
    if(!openFileName.isEmpty())
    {
        QList<CutInfo> cutInfos;
        ReturnType returnType = FileOperate::instance()->openFile(openFileName,cutInfos);
        if(returnType == FILE_ILLEGAL)
        {
            respShowStatusInfo("�ļ���ʽ������������ѡ��");
        }
        else if(returnType == RETURN_OK)
        {
            scene->addItem(cutInfos);

            respShowStatusInfo("�ļ��������!");
        }
    }
    scene->update();
}

//���浱ǰ����ӵĿؼ�
void MainWindow::fileSave()
{
    respRestItemAction();

    QString saveFileName = QFileDialog::getSaveFileName(this,"ѡ��·��");
    if(!saveFileName.isEmpty())
    {
        ReturnType  result = FileOperate::instance()->saveFile(saveFileName,scene->items());
        if(result == RETURN_OK)
        {
            respShowStatusInfo("�ļ�����ɹ�!");
        }
    }
}

//��յ�ǰ�Ŀؼ�
void MainWindow::fileClear()
{
    respRestItemAction();

    int result = QMessageBox::warning(this,"����","�Ƿ���ճ���������?",QMessageBox::Yes,QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        scene->clear();
        respItemSizeChanged(0);
    }
}

void MainWindow::exitApp()
{
    qApp->exit();
}

//Ŀǰ֧��һ���ؼ��ļ��У�ֻ֧�ֳ���ͷ����ؼ�����
void MainWindow::cutItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = typeid(*(selectedItems.first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * item = dynamic_cast<MyItem *>(scene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            deleteItem();
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            delete item;
        }
    }
}

void MainWindow::copyItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = typeid(*(selectedItems.first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * item = dynamic_cast<MyItem *>(scene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->toPlainText();
        }
    }
}

void MainWindow::pasteItem()
{
    scene->addItem(cutTmpInfo,true);
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
    QString itemName = typeid(*selectedItem).name();
    if(itemName == typeid(MyItem).name())
    {
        MyItem * tmp = dynamic_cast<MyItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
    else if(itemName == typeid(MyTextItem).name())
    {
        MyTextItem * tmp = dynamic_cast<MyTextItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
}

//����Ҫ��scene��ɾ��item
void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    foreach(QGraphicsItem * item, selectedItems)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyArrow).name())
        {
            MyArrow * tmp = dynamic_cast<MyArrow *>(item);

            tmp->getStartItem()->removeArrow(tmp);
            tmp->getEndItem()->removeArrow(tmp);
            scene->removeItem(tmp);

            delete tmp;
        }
    }

    selectedItems = scene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyItem).name())
        {
            MyItem * tmp = dynamic_cast<MyItem *>(item);
            tmp->removeArrows();
            scene->removeItem(tmp);
            delete tmp;
        }
    }

    selectedItems = scene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * tmp = dynamic_cast<MyTextItem *>(item);
            scene->removeItem(tmp);
            delete tmp;
        }
    }

    scene->update();
}

//�༭�ı�
void MainWindow::editTextItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    if(selectedItems.size() == 1)
    {
        QString itemName = typeid(*(selectedItems.first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * item = dynamic_cast<MyItem*>(selectedItems.first());

            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());

            //���޸����ֺ���Ҫ���½���Ϣ�������Ҳ�Ĺ�������
            emit initToolBox(selectedItems.size(),item->getProperty());
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
    QWidget * centralWidget = new QWidget;
    QHBoxLayout * layout = new QHBoxLayout;

    SceneWidth = SceneHeight = 5000;

    scene = new MyScene(rightMenu);
    scene->setSceneRect(0,0,SceneWidth,SceneHeight);

    connect(scene,SIGNAL(resetItemAction()),this,SLOT(respRestItemAction()));
    connect(scene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));
    connect(scene,SIGNAL(deleteKeyPress()),this,SLOT(deleteItem()));
    connect(scene,SIGNAL(itemSizeChanged(int)),this,SLOT(respItemSizeChanged(int)));

    view = new QGraphicsView(this);
    view->setScene(scene);
//    view->setDragMode(QGraphicsView::RubberBandDrag);   //������Ӻ�ᵼ��scene�޷�����mousemove�¼�
//    view->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);

    rightToolBox = new RightToolBox;
    connect(this,SIGNAL(initToolBox(int,ItemProperty)),rightToolBox,SLOT(respInitToolBox(int,ItemProperty)));
    connect(rightToolBox,SIGNAL(updateProperty(ItemProperty)),this,SLOT(respPropertyUpdate(ItemProperty)));
    connect(rightToolBox,SIGNAL(deleteCurrItem()),this,SLOT(deleteItem()));
    connect(scene,SIGNAL(selectedItemPosChanged(MyRect)),rightToolBox,SLOT(respItemPosChanged(MyRect)));

    layout->addWidget(view);
    layout->addWidget(rightToolBox);
    centralWidget->setLayout(layout);

    this->setCentralWidget(centralWidget);
}

//��Ӧitem�ı�
void MainWindow::respItemSizeChanged(int size)
{
    bool actionEnabled = size;

    ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(actionEnabled);
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
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(false);
    }
    else if(selectedSize == 1)
    {
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(true);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        QString itemName = typeid(*(scene->selectedItems().first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * myItem = dynamic_cast<MyItem *>(scene->selectedItems().first());
            property = myItem->getProperty();
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(scene->selectedItems().first());
            property = textItem->getProperty();
        }
        else if(itemName == typeid(MyArrow).name())
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(scene->selectedItems().first());
            property = arrowItem->getProperty();
        }
    }
    else
    {
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);
    }

    scene->update();

    emit initToolBox(selectedSize,property);
}

//����item������
void MainWindow::respPropertyUpdate(ItemProperty property)
{
    if(scene->selectedItems().size() == 1)
    {
        QString itemName = typeid(*(scene->selectedItems().first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * myItem = dynamic_cast<MyItem *>(scene->selectedItems().first());
            myItem->setProperty(property);
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(scene->selectedItems().first());
            textItem->setProperty(property);
        }
        else if(itemName == typeid(MyArrow).name())
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(scene->selectedItems().first());
            arrowItem->setProperty(property);
        }

        scene->update();
    }
}

//�����Ҽ��˵�����Ϊÿ������еĹ���
void MainWindow::createContextMenu()
{
    rightMenu = new QMenu;

    rightMenu->addAction(ActionManager::instance()->action(Constants::EDIT_TEXT_ID));
    rightMenu->addSeparator();
    rightMenu->addAction(ActionManager::instance()->action(Constants::CUT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::COPY_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    rightMenu->addSeparator();
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

}

//����������
void MainWindow::createToolBar()
{
    QToolBar * fileBar = addToolBar("File");
    fileBar->addAction(ActionManager::instance()->action(Constants::FILE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::OPEN_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::SAVE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::CLEAR_ID));

    QToolBar * itemBar = addToolBar("Item");
    itemBar->addAction(ActionManager::instance()->action(Constants::ARROW_ID));
    itemBar->addSeparator();
    itemBar->addAction(ActionManager::instance()->action(Constants::SQUARE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::RECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::ROUNDRECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::CIRCLE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::ELLIPSE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::POLYGON_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::TEXT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::VECTOR_LINE_ID));

    QToolBar * editBar = addToolBar("Edit");
    editBar->addAction(ActionManager::instance()->action(Constants::CUT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::COPY_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
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

//����״̬��
void MainWindow::createStatusBar()
{
    QStatusBar * statusBar = new QStatusBar;
    setStatusBar(statusBar);
}

//��ʾ״̬��Ϣ
void MainWindow::respShowStatusInfo(QString text,int time)
{
    statusBar()->showMessage(text);
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
