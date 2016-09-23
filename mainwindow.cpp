#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>

#include "actionmanager.h"
#include "item/myscene.h"
#include "./item/mygraphicsview.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/righttoolbox.h"
#include "./SelfWidget/lefticonwidget.h"
#include "fileoperate.h"
#include "global.h"
#include "util.h"

#include "typeinfo"

using namespace Graphics;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ģ�͹���");

    mySlider = NULL;
    rightToolBox = NULL;
    leftIconWidget = NULL;
    view = NULL;
    GlobalItemZValue = 0;

    createSceneAndView();

    createActionAndMenus();
    createToolBar();
    createStatusBar();

    view->addContextMenuItem();

    showMaximized();

    respItemSizeChanged(0);

    resetEditActionState(false);

    ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
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

    MyAction * undoAction = ActionManager::instance()->crateAction(Constants::UNDO_ID,QIcon(":/images/undo.png"),"����");
    ActionManager::instance()->registerAction(undoAction,MyGraphicsView::instance(),SLOT(undoAndRedoItem()));
    ActionManager::instance()->action(Constants::UNDO_ID)->setEnabled(false);

    MyAction * redoAction = ActionManager::instance()->crateAction(Constants::REDO_ID,QIcon(":/images/redo.png"),"����");
    ActionManager::instance()->registerAction(redoAction,MyGraphicsView::instance(),SLOT(undoAndRedoItem()));
    ActionManager::instance()->action(Constants::REDO_ID)->setEnabled(false);

    MyAction * cutAction = ActionManager::instance()->crateAction(Constants::CUT_ID,QIcon(":/images/cut.png"),"����");
    cutAction->setShortcut(QKeySequence("Ctrl+X"));
    ActionManager::instance()->registerAction(cutAction,MyGraphicsView::instance(),SLOT(cutItem()));

    MyAction * copyAction = ActionManager::instance()->crateAction(Constants::COPY_ID,QIcon(":/images/copy.png"),"����");
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    ActionManager::instance()->registerAction(copyAction,MyGraphicsView::instance(),SLOT(copyItem()));

    MyAction * pasteAction = ActionManager::instance()->crateAction(Constants::PASTE_ID,QIcon(":/images/paste.png"),"ճ��");
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    ActionManager::instance()->registerAction(pasteAction,MyGraphicsView::instance(),SLOT(pasteItem()));

    MyAction * clearPasteAction = ActionManager::instance()->crateAction(Constants::CLEAR_PASTE_ID,QIcon(":/images/clearPaste.png"),"��ռ��а�");
    ActionManager::instance()->registerAction(clearPasteAction,MyGraphicsView::instance(),SLOT(clearPasteItem()));
    ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(false);

    MyAction * rotateLeftAction = ActionManager::instance()->crateAction(Constants::ROTATE_LEFT_ID,QIcon(":/images/rotateLeft.png"),"��ת90��");
    ActionManager::instance()->registerAction(rotateLeftAction,MyGraphicsView::instance(),SLOT(rotateItem()));

    MyAction * rotateRightAction = ActionManager::instance()->crateAction(Constants::ROTATE_RIGHT_ID,QIcon(":/images/rotateRight.png"),"��ת90��");
    ActionManager::instance()->registerAction(rotateRightAction,MyGraphicsView::instance(),SLOT(rotateItem()));

    MyAction * bringFrontAction = ActionManager::instance()->crateAction(Constants::BRING_FRONT_ID,QIcon(":/images/sendtoback.png"),"���ڶ���");
    ActionManager::instance()->registerAction(bringFrontAction,MyGraphicsView::instance(),SLOT(bringZItem()));

    MyAction * bringBackAction = ActionManager::instance()->crateAction(Constants::BRING_BACK_ID,QIcon(":/images/bringtofront.png"),"���ڵײ�");
    ActionManager::instance()->registerAction(bringBackAction,MyGraphicsView::instance(),SLOT(bringZItem()));

    MyAction * lockAction = ActionManager::instance()->crateAction(Constants::LOCK_ID,QIcon(":/images/lock.png"),"����");
    ActionManager::instance()->registerAction(lockAction,MyGraphicsView::instance(),SLOT(lockAndunlockItem()));

    MyAction * unlockAction = ActionManager::instance()->crateAction(Constants::UNLOCK_ID,QIcon(":/images/unlock.png"),"����");
    ActionManager::instance()->registerAction(unlockAction,MyGraphicsView::instance(),SLOT(lockAndunlockItem()));

    MyAction * deleteAction = ActionManager::instance()->crateAction(Constants::DELETE_ID,QIcon(":/images/delete.png"),"ɾ��");
    ActionManager::instance()->registerAction(deleteAction,MyGraphicsView::instance(),SLOT(deleteItem()));

    MyAction * editTextAction = ActionManager::instance()->crateAction(Constants::EDIT_TEXT_ID,QIcon(":/images/editText.png"),"�༭�ı�");
    editTextAction->setShortcut(QKeySequence("Ctrl+T"));
    ActionManager::instance()->registerAction(editTextAction,MyGraphicsView::instance(),SLOT(editTextItem()));

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
    editMenu->addAction(lockAction);
    editMenu->addAction(unlockAction);
    editMenu->addAction(deleteAction);


    MyAction * dragAbleAction = ActionManager::instance()->crateAction(Constants::DRAG_ABLE_ID,QIcon(":/images/dragable.png"),"����������ק");
    ActionManager::instance()->registerAction(dragAbleAction,MyGraphicsView::instance(),SLOT(setViewDragEnable(bool)),true);
    ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setChecked(true);

    QMenu * itemMenu = menuBar()->addMenu("��Ŀ(&I)");

    MyAction * arrowAction = ActionManager::instance()->crateAction(Constants::ARROW_ID,QIcon(":/images/pointer.png"),"��ͷ");
    ActionManager::instance()->registerAction(arrowAction,this,SLOT(recordClickedItem()),true);
    arrowAction->setType(GRA_NONE);
    arrowAction->setChecked(true);

//    MyAction * squareAction = ActionManager::instance()->crateAction(Constants::SQUARE_ID,QIcon(":/images/square.png"),"������");
//    ActionManager::instance()->registerAction(squareAction,this,SLOT(recordClickedItem()),true);
//    squareAction->setType(GRA_SQUARE);

//    MyAction * rectAction = ActionManager::instance()->crateAction(Constants::RECT_ID,QIcon(":/images/rectange.png"),"����");
//    ActionManager::instance()->registerAction(rectAction,this,SLOT(recordClickedItem()),true);
//    rectAction->setType(GRA_RECT);

//    MyAction * roundRectAction = ActionManager::instance()->crateAction(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"Բ�Ǿ���");
//    ActionManager::instance()->registerAction(roundRectAction,this,SLOT(recordClickedItem()),true);
//    roundRectAction->setType(GRA_ROUND_RECT);

//    MyAction * circleAction = ActionManager::instance()->crateAction(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"Բ");
//    ActionManager::instance()->registerAction(circleAction,this,SLOT(recordClickedItem()),true);
//    circleAction->setType(GRA_CIRCLE);

//    MyAction * ellipseAction = ActionManager::instance()->crateAction(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"��Բ");
//    ActionManager::instance()->registerAction(ellipseAction,this,SLOT(recordClickedItem()),true);
//    ellipseAction->setType(GRA_ELLIPSE);

//    MyAction * polygonAction = ActionManager::instance()->crateAction(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"����");
//    ActionManager::instance()->registerAction(polygonAction,this,SLOT(recordClickedItem()),true);
//    polygonAction->setType(GRA_POLYGON);

    MyAction * lineAction = ActionManager::instance()->crateAction(Constants::LINE_ID,QIcon(":/images/linepointer.png"),"����");
    ActionManager::instance()->registerAction(lineAction,this,SLOT(recordClickedItem()),true);
    lineAction->setType(GRA_LINE);

    MyAction * vectorLineAction = ActionManager::instance()->crateAction(Constants::VECTOR_LINE_ID,QIcon(":/images/vectorLine.png"),"������");
    ActionManager::instance()->registerAction(vectorLineAction,this,SLOT(recordClickedItem()),true);
    vectorLineAction->setType(GRA_VECTOR_LINE);

    MyAction * textAction = ActionManager::instance()->crateAction(Constants::TEXT_ID,QIcon(":/images/text.png"),"����");
    ActionManager::instance()->registerAction(textAction,this,SLOT(recordClickedItem()),true);
    textAction->setType(GRA_TEXT);

    //���ڿ���һ����ֻ��һ��״̬��ѡ��
    itemGroup = new QActionGroup(this);

    itemGroup->addAction(arrowAction);
//    itemGroup->addAction(squareAction);
//    itemGroup->addAction(rectAction);
//    itemGroup->addAction(roundRectAction);
//    itemGroup->addAction(circleAction);
//    itemGroup->addAction(ellipseAction);
//    itemGroup->addAction(polygonAction);
    itemGroup->addAction(textAction);
    itemGroup->addAction(lineAction);
    itemGroup->addAction(vectorLineAction);

    itemMenu->addAction(arrowAction);
//    itemMenu->addAction(squareAction);
//    itemMenu->addAction(rectAction);
//    itemMenu->addAction(roundRectAction);
//    itemMenu->addAction(circleAction);
//    itemMenu->addAction(ellipseAction);
//    itemMenu->addAction(polygonAction);
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

    if(MyGraphicsView::instance()->scene()->items().size()>0)
    {
        fileClear();
    }

    QString openFileName = QFileDialog::getOpenFileName(this,"ѡ����ļ�","","Files(*"+SaveFileSuffix+")");
    if(!openFileName.isEmpty())
    {
        QList<CutInfo *> cutInfos;
        ReturnType returnType = FileOperate::instance()->openFile(openFileName,cutInfos);
        if(returnType == FILE_ILLEGAL)
        {
            respShowStatusInfo("�ļ���ʽ������������ѡ��");
        }
        else if(returnType == RETURN_OK)
        {
            MyGraphicsView::instance()->scene()->addItem(cutInfos);

            respShowStatusInfo("�ļ��������!");
        }
    }
    MyGraphicsView::instance()->scene()->update();
}

//���浱ǰ����ӵĿؼ�
void MainWindow::fileSave()
{
    respRestItemAction();

    QString saveFileName = QFileDialog::getSaveFileName(this,"ѡ��·��");
    if(!saveFileName.isEmpty())
    {
        ReturnType  result = FileOperate::instance()->saveFile(saveFileName,MyGraphicsView::instance()->scene()->items());
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
        view->clearItems();
        respItemSizeChanged(0);
        resetEditActionState(false);
        Util::resetGlobalZValue();
    }
}

void MainWindow::exitApp()
{
    qApp->exit();
}

//��ȡ����ؼ�������
void MainWindow::recordClickedItem()
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

    view = new MyGraphicsView(this);

    rightToolBox = new RightToolBox;
    connect(view,SIGNAL(initToolBox(int,ItemProperty)),rightToolBox,SLOT(respInitToolBox(int,ItemProperty)));
    connect(rightToolBox,SIGNAL(updateProperty(ItemProperty)),MyGraphicsView::instance(),SLOT(respPropertyUpdate(ItemProperty)));
    connect(rightToolBox,SIGNAL(deleteCurrItem()),MyGraphicsView::instance(),SLOT(deleteItem()));
    connect(view,SIGNAL(itemPropChanged(ItemProperty)),rightToolBox,SLOT(respItemPropChanged(ItemProperty)));

    leftIconWidget = new LeftIconWidget;

    layout->addWidget(leftIconWidget);
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

    if(view->scene()->selectedItems().size() > 0)
    {
        resetEditActionState(actionEnabled);
    }
}

//���ñ༭action��״̬
void MainWindow::resetEditActionState(bool state)
{
    ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(state);

    ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(state);
}

//����scene���һ�ʱ����item�������е�״̬�ָ�����ͷ״̬
void MainWindow::respRestItemAction()
{
    ActionManager::instance()->action(Constants::ARROW_ID)->setChecked(true);
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
//    itemBar->addAction(ActionManager::instance()->action(Constants::SQUARE_ID));
//    itemBar->addAction(ActionManager::instance()->action(Constants::RECT_ID));
//    itemBar->addAction(ActionManager::instance()->action(Constants::ROUNDRECT_ID));
//    itemBar->addAction(ActionManager::instance()->action(Constants::CIRCLE_ID));
//    itemBar->addAction(ActionManager::instance()->action(Constants::ELLIPSE_ID));
//    itemBar->addAction(ActionManager::instance()->action(Constants::POLYGON_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::TEXT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::VECTOR_LINE_ID));

    QToolBar * editBar = addToolBar("Edit");
    editBar->addAction(ActionManager::instance()->action(Constants::UNDO_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::REDO_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::CUT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::COPY_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::CLEAR_PASTE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::LOCK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::UNLOCK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    QToolBar * sceneBar = addToolBar("Scene");

    mySlider = new MySlider;
    connect(mySlider,SIGNAL(scaleView(int)),MyGraphicsView::instance(),SLOT(sceneScaled(int)));

    sceneBar->addAction(ActionManager::instance()->action(Constants::DRAG_ABLE_ID));
    sceneBar->addSeparator();
    sceneBar->addWidget(mySlider);
}

//����״̬��
void MainWindow::createStatusBar()
{
    QStatusBar * statusBar = new QStatusBar;
    setStatusBar(statusBar);
}

//��ʾ״̬��Ϣ
void MainWindow::respShowStatusInfo(QString text)
{
    statusBar()->showMessage(text);
}

MainWindow::~MainWindow()
{
    if(mySlider)
    {
        delete mySlider;
        mySlider = NULL;
    }

    if(leftIconWidget)
    {
        delete leftIconWidget;
        leftIconWidget = NULL;
    }

    if(rightToolBox)
    {
        delete rightToolBox;
        rightToolBox = NULL;
    }

    if(view)
    {
        delete view;
        view = NULL;
    }

    delete ui;
}
