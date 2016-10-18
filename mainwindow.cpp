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
#include <QKeyEvent>
#include <QDebug>

#include "./manager/actionmanager.h"
#include "item/myscene.h"
#include "./item/mygraphicsview.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/lefticonwidget.h"
#include "./SelfWidget/hidesplit.h"
#include "./SelfWidget/righttoolbox.h"
#include "./SelfWidget/mypageswitch.h"
#include "./SelfWidget/serviceview.h"
#include "./manager/MyLineComboBox.h"
#include "./simulate/simulatecontrolpanel.h"
#include "fileoperate.h"
#include "global.h"
#include "util.h"

using namespace Graphics;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GlobalMainWindow = this;

    mySlider = NULL;
    leftIconWidget = NULL;
    rightToolBox = NULL;
    view = NULL;
    simulatePanel = NULL;

    fileBar = NULL;
    itemBar = NULL;
    sceneBar = NULL;
    editBar = NULL;
    databaseBar = NULL;

    GlobalItemZValue = 0;

    createSceneAndView();

    createLineComboBox();
    createActionAndMenus();
    createToolBar();
    createStatusBar();

    view->addContextMenuItem();
    view->addViewContextMenu();

    showMaximized();

    respItemSizeChanged(0);

    resetEditActionState(false);

    ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();

    ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->setChecked(true);
}

//�������ڵĲ˵���������Ӧ�¼�
void MainWindow::createActionAndMenus()
{
    fileMenu = menuBar()->addMenu("�ļ�(&F)");

    MyAction * newAction = ActionManager::instance()->crateAction(Constants::FILE_ID,QIcon(":/images/new.png"),"�½�������");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    ActionManager::instance()->registerAction(newAction,MyPageSwitch::instance(),SLOT(addPage()));

    MyAction * saveAction = ActionManager::instance()->crateAction(Constants::SAVE_ID,QIcon(":/images/save.png"),"����");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    ActionManager::instance()->registerAction(saveAction,this,SLOT(fileSave()));

    MyAction * openAction = ActionManager::instance()->crateAction(Constants::OPEN_ID,QIcon(":/images/open.png"),"��");
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    ActionManager::instance()->registerAction(openAction,this,SLOT(fileOpen()));

    MyAction * clearAction = ActionManager::instance()->crateAction(Constants::CLEAR_ID,QIcon(":/images/clear.png"),"���");
    ActionManager::instance()->registerAction(clearAction,this,SLOT(fileClear()));

    MyAction * exitAction = ActionManager::instance()->crateAction(Constants::EXIT_ID,"�˳�");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    ActionManager::instance()->registerAction(exitAction,this,SLOT(exitApp()));

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(clearAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu("�༭(&E)");

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
    lockAction->setShortcut(QKeySequence("Ctrl+L"));
    ActionManager::instance()->registerAction(lockAction,MyGraphicsView::instance(),SLOT(lockAndunlockItem()));

    MyAction * unlockAction = ActionManager::instance()->crateAction(Constants::UNLOCK_ID,QIcon(":/images/unlock.png"),"����");
    unlockAction->setShortcut(QKeySequence("Ctrl+Shift+L"));
    ActionManager::instance()->registerAction(unlockAction,MyGraphicsView::instance(),SLOT(lockAndunlockItem()));

    MyAction * deleteAction = ActionManager::instance()->crateAction(Constants::DELETE_ID,QIcon(":/images/delete.png"),"ɾ��");
    ActionManager::instance()->registerAction(deleteAction,MyGraphicsView::instance(),SLOT(deleteItem()));

    MyAction * editTextAction = ActionManager::instance()->crateAction(Constants::EDIT_TEXT_ID,QIcon(":/images/editText.png"),"�༭����");
    editTextAction->setShortcut(QKeySequence("Ctrl+T"));
    ActionManager::instance()->registerAction(editTextAction,MyGraphicsView::instance(),SLOT(editTextItem()));

    MyAction * propertyEditAction = ActionManager::instance()->crateAction(Constants::PROPERTY_EDIT_ID,QIcon(":/images/editProp.png"),"�༭����");
    propertyEditAction->setShortcut(QKeySequence("Ctrl+E"));
    ActionManager::instance()->registerAction(propertyEditAction,MyGraphicsView::instance(),SLOT(editPropertyItem()));

    editMenu->addAction(editTextAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
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

    itemMenu = menuBar()->addMenu("����(&I)");

    MyAction * arrowAction = ActionManager::instance()->crateAction(Constants::ARROW_ID,QIcon(":/images/pointer.png"),"��ͷ");
    ActionManager::instance()->registerAction(arrowAction,this,SLOT(recordClickedItem()),true);
    arrowAction->setType(GRA_NONE);
    arrowAction->setChecked(true);

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
    itemGroup->addAction(textAction);
    itemGroup->addAction(lineAction);
    itemGroup->addAction(vectorLineAction);

    itemMenu->addAction(arrowAction);
    itemMenu->addAction(textAction);
    itemMenu->addAction(lineAction);
    itemMenu->addAction(vectorLineAction);

    MyAction * buildModelAction = ActionManager::instance()->crateAction(Constants::BUILD_MODEL_ID,QIcon(":/images/buildmodel.png"),"��ģ");
    buildModelAction->setShortcut(QKeySequence("Ctrl+M"));
    ActionManager::instance()->registerAction(buildModelAction,this,SLOT(switchWorkModel()),true);

    MyAction *simulateAction = ActionManager::instance()->crateAction(Constants::SIMLUATE_ID,QIcon(":/images/simulate.png"),"����");
    simulateAction->setShortcut(QKeySequence("Ctrl+Shift+M"));
    ActionManager::instance()->registerAction(simulateAction,this,SLOT(switchWorkModel()),true);

    workModelGroup = new QActionGroup(this);
    workModelGroup->addAction(buildModelAction);
    workModelGroup->addAction(simulateAction);

    widgetMenu = menuBar()->addMenu("����(&W)");
    //�����ڲ˵�����
    MyAction * fullScreenAction = ActionManager::instance()->crateAction(Constants::FULL_SCREEN_ID,QIcon(":/images/fullscreen.png"),"ȫ��");
    fullScreenAction->setShortcut(QKeySequence("Ctrl+Shift+F11"));
    ActionManager::instance()->registerAction(fullScreenAction,this,SLOT(switchFullScreen()));

    MyAction * hideIconAction = ActionManager::instance()->crateAction(Constants::HIDE_ICON_ID,QIcon(""),"�������ؼ���");
    hideIconAction->setShortcut(QKeySequence("Ctrl+L"));
    ActionManager::instance()->registerAction(hideIconAction,this,SLOT(hideSubWidget()));

    MyAction * hideToolAction = ActionManager::instance()->crateAction(Constants::HIDE_TOOL_ID,QIcon(""),"�������Ա༭��");
    hideToolAction->setShortcut(QKeySequence("Ctrl+R"));
    ActionManager::instance()->registerAction(hideToolAction,this,SLOT(hideSubWidget()));

    widgetMenu->addAction(fullScreenAction);
    widgetMenu->addAction(hideIconAction);
    widgetMenu->addAction(hideToolAction);
    widgetMenu->addSeparator();
    widgetMenu->addAction(buildModelAction);
    widgetMenu->addAction(simulateAction);

    helpMenu = menuBar()->addMenu("����(&H)");
    //�������˵�����
    MyAction * supportAction = ActionManager::instance()->crateAction(Constants::TEC_SUPPORT_ID,QIcon(":/images/getsupport.png"),"����֧��");
//    supportAction->setShortcut(QKeySequence("Ctrl+Q"));
    ActionManager::instance()->registerAction(supportAction,this,SLOT(getTecSupport()));

    MyAction * aboutAction = ActionManager::instance()->crateAction(Constants::ABOUT_ID,QIcon(":/images/about.png"),"����");
    aboutAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(aboutAction,this,SLOT(showAbout()));

    helpMenu->addAction(supportAction);
    helpMenu->addAction(aboutAction);

    MyAction * databaseViewAction = ActionManager::instance()->crateAction(Constants::DATABASE_VIEW,QIcon(":/images/database_view.png"),"�鿴���ݿ�");
//    databaseViewAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(databaseViewAction,ServiceView::instance(),SLOT(viewDatabaseContent()));

    MyAction * databaseRefreshAction = ActionManager::instance()->crateAction(Constants::DATABASE_REFRESH,QIcon(":/images/database_refresh.png"),"ˢ�����ݿ�");
//    databaseViewAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(databaseRefreshAction,ServiceView::instance(),SLOT(refreshDatabaseContent()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //ȫ��/��ȫ��
    if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_F11)
    {
        switchFullScreen();
    }
    //�������
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_L)
    {
        SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setContainerVisible();
    }
    //�����Ҳ�
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_R)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
             SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();

             if(SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->getContainer()->isVisible())
             {
                 ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("�������Ա༭��");
             }
             else
             {
                 ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("��ʾ���Ա༭��");
             }
        }
        else
        {
            SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setContainerVisible();

            if(SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->getContainer()->isVisible())
            {
                ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("����������");
            }
            else
            {
                ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("��ʾ������");
            }
        }
    }
    //�رչ�����
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            MyPageSwitch::instance()->closePage();
        }
    }
    //����ģʽ�л�[��ģ->����]
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_M)
    {
        if(!ActionManager::instance()->action(Constants::SIMLUATE_ID)->isChecked())
        {
            ActionManager::instance()->action(Constants::SIMLUATE_ID)->setChecked(true);
        }
    }
    //[����->��ģ]
    else if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_M)
    {
        if(!ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->isChecked())
        {
            ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->setChecked(true);
        }
    }

    QMainWindow::keyPressEvent(event);
}

//�л�ȫ��
void MainWindow::switchFullScreen()
{
    if(isFullScreen())
    {
        showMaximized();
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setIcon(QIcon(":/images/fullscreen.png"));
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setText("ȫ��");
    }
    else
    {
        showFullScreen();
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setIcon(QIcon(":/images/exit_fullscreen.png"));
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setText("�˳�ȫ��");
    }
}

//��ʾ/�������ҵĿؼ�
void MainWindow::hideSubWidget()
{
    QString objName = QObject::sender()->objectName();

    if(GlobalWindowState == WINDOW_BUILD_MODEL)
    {
        if(objName == QString(Constants::HIDE_ICON_ID))
        {
            SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setContainerVisible();
        }
        else if(objName == QString(Constants::HIDE_TOOL_ID))
        {
            SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();
        }
    }
    else
    {
        if(objName == QString(Constants::HIDE_TOOL_ID))
        {
            SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setContainerVisible();
        }
    }
}

//��ȡ����֧��
void MainWindow::getTecSupport()
{

}

//��ʾ����
void MainWindow::showAbout()
{
    qApp->aboutQt();
}

//�л�����ģʽ����ģ/���ݡ�
void MainWindow::switchWorkModel()
{
    QString objName = QObject::sender()->objectName();
    bool enable = false;

    MyGraphicsView::instance()->respResetSimluate();

    if(objName == QString(Constants::BUILD_MODEL_ID))
    {
        enable = true;
        GlobalWindowState = WINDOW_BUILD_MODEL;
        setWindowTitle("�����ģ������Эͬ���ù���-��ģ");
    }
    else
    {
        GlobalWindowState = WINDOW_SIMULATE;
        setWindowTitle("�����ģ������Эͬ���ù���-����");
    }

    MyGraphicsView::instance()->addContextMenuItem();

    SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setVisible(enable);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setVisible(enable);
    SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setVisible(!enable);
    MyPageSwitch::instance()->setVisible(enable);

    if(!fileBar)
    {
        return;
    }

    fileMenu->setEnabled(enable);
    editMenu->setEnabled((enable));
    itemMenu->setEnabled((enable));

    fileBar->setEnabled(enable);
    itemBar->setEnabled(enable);
    editBar->setEnabled(enable);
    databaseBar->setEnabled(!enable);
}

//�򿪱��ر�����ļ���������ʾ�Ƿ�Ҫ���浱ǰ��ӵĿؼ�
void MainWindow::fileOpen()
{
    respRestItemAction();

    MY_ASSERT(MyGraphicsView::instance()->scene())

    fileClear();

    QString openFileName = QFileDialog::getOpenFileName(this,"ѡ����ļ�","","Files(*"+SaveFileSuffix+")");
    if(!openFileName.isEmpty())
    {
        QList<CutInfo *> cutInfos;
        ReturnType returnType = FileOperate::instance()->openFile(openFileName,cutInfos);
        if(returnType == FILE_ILLEGAL)
        {
            respShowStatusInfo("�ļ���ʽ������������ѡ��");
        }
        else if(returnType == RETURN_SUCCESS)
        {
            MyGraphicsView::instance()->scene()->addItem(cutInfos);
            MyGraphicsView::instance()->setKeyCtrlStated(false);

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
        if(result == RETURN_SUCCESS)
        {
            respShowStatusInfo("�ļ�����ɹ�!");
        }
    }
}

//��յ�ǰ�Ŀؼ�
void MainWindow::fileClear()
{
    respRestItemAction();

    if(view->getItemSize() > 1)
    {
        int result = QMessageBox::warning(this,"����","�Ƿ���ճ���������?",QMessageBox::Yes,QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            view->clearItems();
            respItemSizeChanged(0);
            resetEditActionState(false);
            Util::resetGlobalZValue();
        }
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

//ҳ��ɾ���󣬽������״̬�л����ؼ������Ҳ�ؼ���
void MainWindow::respDeletePage()
{
    if(MyGraphicsView::instance()->scene() == NULL)
    {
        ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(false);
    }

    rightToolBox->enableButtState(false);
}

//������������ͼ
void MainWindow::createSceneAndView()
{
    QWidget * centralWidget = new QWidget;
    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(LAYOUT_SPACING);

    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(1,1,1,1);
    view = new MyGraphicsView(this);

    vLayout->addWidget(MyPageSwitch::instance());
    vLayout->addWidget(view);
    MyPageSwitch::instance()->addPage();

    connect(MyPageSwitch::instance(),SIGNAL(deletePage()),this,SLOT(respDeletePage()));

    leftIconWidget = new LeftIconWidget;

    rightToolBox = new RightToolBox;
    connect(MyGraphicsView::instance(),SIGNAL(initToolBox(int,ItemProperty)),rightToolBox,SLOT(respInitToolBox(int,ItemProperty)));
    connect(rightToolBox,SIGNAL(updateProperty(ItemProperty)),MyGraphicsView::instance(),SLOT(respPropertyUpdate(ItemProperty)));
    connect(rightToolBox,SIGNAL(deleteCurrItem()),MyGraphicsView::instance(),SLOT(deleteItem()));
    connect(MyGraphicsView::instance(),SIGNAL(itemPropChanged(ItemProperty)),rightToolBox,SLOT(respItemPropChanged(ItemProperty)));

    simulatePanel = new SimulateControlPanel;
    connect(simulatePanel,SIGNAL(resetSimluate()),view,SLOT(respResetSimluate()));

    layout->addWidget(SplitManager::instance()->addSplit(QString(Constants::HIDE_ICON_ID),SPLIT_RIGHT,leftIconWidget));
    layout->addLayout(vLayout);
    layout->addWidget(SplitManager::instance()->addSplit(QString(Constants::HIDE_TOOL_ID),SPLIT_LEFT,rightToolBox));
    layout->addWidget(SplitManager::instance()->addSplit(QString(Constants::HIDE_SIMULATE_ID),SPLIT_LEFT,simulatePanel));
    centralWidget->setLayout(layout);

    SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setFixedWidth(162);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setFixedWidth(310);
    SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setFixedWidth(310);

    this->setCentralWidget(centralWidget);
}

//��Ӧitem�ı�
void MainWindow::respItemSizeChanged(int size)
{
    MY_ASSERT(view->scene())
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

//���������Զ����������������Ҫʹ�����֣���ôֱ�����NULL
void MainWindow::createLineComboBox()
{
    MyLineComboBox * startLineBox = ComboBoxManager::instance()->addItem(Constants::LEFT_LINE_ID);
    MyLineComboBox * endLineBox = ComboBoxManager::instance()->addItem(Constants::RIGHT_LINE_ID);

    startLineBox->addItem(QIcon(":/images/arrow/leftLine.png"),NULL);
    startLineBox->addItem(QIcon(":/images/arrow/leftArrow.png"),NULL);
    startLineBox->addItem(QIcon(":/images/arrow/leftSolidTriangle.png"),NULL);

    endLineBox->addItem(QIcon(":/images/arrow/rightLine.png"),NULL);
    endLineBox->addItem(QIcon(":/images/arrow/rightArrow.png"),NULL);
    endLineBox->addItem(QIcon(":/images/arrow/rightSolidTriangle.png"),NULL);

    connect(startLineBox,SIGNAL(indexChanged(int)),MyGraphicsView::instance(),SLOT(setSelectedLineType(int)));
    connect(endLineBox,SIGNAL(indexChanged(int)),MyGraphicsView::instance(),SLOT(setSelectedLineType(int)));
    startLineBox->setEnabled(false);
    endLineBox->setEnabled(false);
}

//����������
void MainWindow::createToolBar()
{
    QToolBar * modelBar = addToolBar("Model");
    modelBar->addAction(ActionManager::instance()->action(Constants::BUILD_MODEL_ID));
    modelBar->addAction(ActionManager::instance()->action(Constants::SIMLUATE_ID));

    fileBar = addToolBar("File");
    fileBar->addAction(ActionManager::instance()->action(Constants::FILE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::OPEN_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::SAVE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::CLEAR_ID));

    itemBar = addToolBar("Item");
    itemBar->addAction(ActionManager::instance()->action(Constants::ARROW_ID));
    itemBar->addSeparator();
    itemBar->addAction(ActionManager::instance()->action(Constants::TEXT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::VECTOR_LINE_ID));

    editBar = addToolBar("Edit");
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
    editBar->addWidget(ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID));
    editBar->addWidget(ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    sceneBar = addToolBar("View");

    mySlider = new MySlider;
    connect(mySlider,SIGNAL(scaleView(int)),MyGraphicsView::instance(),SLOT(sceneScaled(int)));
    connect(view,SIGNAL(scaleValue(int)),mySlider,SLOT(respSetScaleValue(int)));
    connect(view,SIGNAL(zoomIn()),mySlider,SLOT(respZoomIn()));
    connect(view,SIGNAL(zoomOut()),mySlider,SLOT(respZoomOut()));

    sceneBar->addAction(ActionManager::instance()->action(Constants::DRAG_ABLE_ID));
    sceneBar->addSeparator();
    sceneBar->addWidget(mySlider);

    databaseBar = addToolBar("Database");
    databaseBar->addAction(ActionManager::instance()->action(Constants::DATABASE_VIEW));
    databaseBar->addAction(ActionManager::instance()->action(Constants::DATABASE_REFRESH));
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

    if(view)
    {
        delete view;
        view = NULL;
    }

    delete ui;
}
