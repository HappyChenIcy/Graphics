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
#include <QMessageBox>

#include "./manager/actionmanager.h"
#include "./manager/menumanager.h"
#include "./item/myscene.h"
#include "./item/mygraphicsview.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/lefticonwidget.h"
#include "./SelfWidget/hidesplit.h"
#include "./SelfWidget/righttoolbox.h"
#include "./SelfWidget/mypageswitch.h"
#include "./SelfWidget/serviceview.h"
#include "./SelfWidget/aboutsoft.h"
#include "./manager/mylinecombobox.h"
#include "./simulate/simulatecontrolpanel.h"
#include "./project/myprowizard.h"
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
    GlobalIsSimulateState = false;

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

    MyPageSwitch::instance()->addSwitchContextMenu();
    MyPageSwitch::instance()->addPage();

    view->addContextMenuItem();
    view->addViewContextMenu();
    view->respItemSizeChanged(0);

    showMaximized();

    resetEditActionState(false);

    ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();

    ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->setChecked(true);
}

//�������ڵĲ˵���������Ӧ�¼�
void MainWindow::createActionAndMenus()
{
    MyAction * newProAction = ActionManager::instance()->crateAction(Constants::PROJECT_ID,QIcon(":/images/project.png"),"�½�����");
    ActionManager::instance()->registerAction(newProAction,MyProWizard::instance(),SLOT(exec()));

    MyAction * newFileAction = ActionManager::instance()->crateAction(Constants::FILE_ID,QIcon(":/images/new.png"),"�½�������");
    newFileAction->setShortcut(QKeySequence("Ctrl+N"));
    ActionManager::instance()->registerAction(newFileAction,MyPageSwitch::instance(),SLOT(addPage()));

    MyAction * saveAction = ActionManager::instance()->crateAction(Constants::SAVE_ID,QIcon(":/images/save.png"),"����");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    ActionManager::instance()->registerAction(saveAction,MyGraphicsView::instance(),SLOT(fileSave()));

    MyAction * saveAsAction = ActionManager::instance()->crateAction(Constants::SAVE_AS_ID,QIcon(""),"���Ϊ");
    saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    ActionManager::instance()->registerAction(saveAsAction,MyGraphicsView::instance(),SLOT(fileSaveAs()));

    MyAction * openAction = ActionManager::instance()->crateAction(Constants::OPEN_ID,QIcon(":/images/open.png"),"��");
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    ActionManager::instance()->registerAction(openAction,this,SLOT(fileOpen()));

    MyAction * clearAction = ActionManager::instance()->crateAction(Constants::CLEAR_ID,QIcon(":/images/clear.png"),"���");
    ActionManager::instance()->registerAction(clearAction,this,SLOT(fileClear()));

    MyAction * exitAction = ActionManager::instance()->crateAction(Constants::EXIT_ID,"�˳�");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    ActionManager::instance()->registerAction(exitAction,this,SLOT(close()));

    MyMenu * fileMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_FILE_MENU,QString("�ļ�(&F)"));
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(newProAction);
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(clearAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    MyAction * undoAction = ActionManager::instance()->crateAction(Constants::UNDO_ID,QIcon(":/images/undo.png"),"����");
    ActionManager::instance()->registerAction(undoAction,MyGraphicsView::instance(),SLOT(undoAndRedoItem()));
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    undoAction->setEnabled(false);

    MyAction * redoAction = ActionManager::instance()->crateAction(Constants::REDO_ID,QIcon(":/images/redo.png"),"����");
    ActionManager::instance()->registerAction(redoAction,MyGraphicsView::instance(),SLOT(undoAndRedoItem()));
    redoAction->setShortcut(QKeySequence("Ctrl+Y"));
    redoAction->setEnabled(false);

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

    MyAction * rotateLeftAction = ActionManager::instance()->crateAction(Constants::ROTATE_LEFT_ID,QIcon(":/images/rotateLeft.png"),"��ת90��(��ʱ��)");
    ActionManager::instance()->registerAction(rotateLeftAction,MyGraphicsView::instance(),SLOT(rotateItem()));

    MyAction * rotateRightAction = ActionManager::instance()->crateAction(Constants::ROTATE_RIGHT_ID,QIcon(":/images/rotateRight.png"),"��ת90��(˳ʱ��)");
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
    deleteAction->setShortcut(QKeySequence("Delete"));

    MyAction * editTextAction = ActionManager::instance()->crateAction(Constants::EDIT_TEXT_ID,QIcon(":/images/editText.png"),"�༭����");
    editTextAction->setShortcut(QKeySequence("Ctrl+T"));
    ActionManager::instance()->registerAction(editTextAction,MyGraphicsView::instance(),SLOT(respEditText()));

    MyAction * propertyEditAction = ActionManager::instance()->crateAction(Constants::PROPERTY_EDIT_ID,QIcon(":/images/editProp.png"),"�༭����");
    propertyEditAction->setShortcut(QKeySequence("Ctrl+E"));
    ActionManager::instance()->registerAction(propertyEditAction,MyGraphicsView::instance(),SLOT(editPropertyItem()));

    MyMenu * editMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_EDIT_MENU,QString("�༭(&E)"));
    menuBar()->addMenu(editMenu);

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

    MyMenu * itemMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_ITEM_MENU,QString("����(&I)"));
    menuBar()->addMenu(itemMenu);

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

    //�����ڲ˵�����
    MyAction * fullScreenAction = ActionManager::instance()->crateAction(Constants::FULL_SCREEN_ID,QIcon(":/images/fullscreen.png"),"ȫ��");
    fullScreenAction->setShortcut(QKeySequence("Ctrl+Shift+F11"));
    ActionManager::instance()->registerAction(fullScreenAction,this,SLOT(switchFullScreen()));

    MyAction * hideIconAction = ActionManager::instance()->crateAction(Constants::HIDE_ICON_ID,QIcon(""),"���ؿؼ���");
    hideIconAction->setShortcut(QKeySequence("Alt+L"));
    ActionManager::instance()->registerAction(hideIconAction,this,SLOT(hideSubWidget()));

    MyAction * hideToolAction = ActionManager::instance()->crateAction(Constants::HIDE_TOOL_ID,QIcon(""),"�������Ա༭��");
    hideToolAction->setShortcut(QKeySequence("Alt+R"));
    ActionManager::instance()->registerAction(hideToolAction,this,SLOT(hideSubWidget()));

    MyAction * hideSpaceAction = ActionManager::instance()->crateAction(Constants::HIDE_WORKSPACE_ID,QIcon(""),"���ع�����");
    hideSpaceAction->setShortcut(QKeySequence("Alt+U"));
    ActionManager::instance()->registerAction(hideSpaceAction,this,SLOT(hideSubWidget()));

    MyMenu * widgetMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_WIDGET_MENU,QString("����(&W)"));
    menuBar()->addMenu(widgetMenu);
    widgetMenu->addAction(fullScreenAction);
    widgetMenu->addAction(hideIconAction);
    widgetMenu->addAction(hideToolAction);
    widgetMenu->addAction(hideSpaceAction);
    widgetMenu->addSeparator();
    widgetMenu->addAction(buildModelAction);
    widgetMenu->addAction(simulateAction);

    //�������˵�����
    MyAction * supportAction = ActionManager::instance()->crateAction(Constants::TEC_SUPPORT_ID,QIcon(":/images/getsupport.png"),"����֧��");
//    supportAction->setShortcut(QKeySequence("Ctrl+Q"));
    ActionManager::instance()->registerAction(supportAction,this,SLOT(getTecSupport()));

    MyAction * aboutAction = ActionManager::instance()->crateAction(Constants::ABOUT_ID,QIcon(":/images/about.png"),"����");
    aboutAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(aboutAction,this,SLOT(showAbout()));

    MyMenu * helpMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_HELP_MENU,QString("����(&H)"));
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(supportAction);
    helpMenu->addAction(aboutAction);

    MyAction * databaseViewAction = ActionManager::instance()->crateAction(Constants::DATABASE_VIEW,QIcon(":/images/database_view.png"),"�鿴���ݿ�");
//    databaseViewAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(databaseViewAction,ServiceView::instance(),SLOT(viewDatabaseContent()));

    MyAction * databaseRefreshAction = ActionManager::instance()->crateAction(Constants::DATABASE_REFRESH,QIcon(":/images/database_refresh.png"),"ˢ�����ݿ�");
    ActionManager::instance()->registerAction(databaseRefreshAction,ServiceView::instance(),SLOT(refreshDatabaseContent()));

    //�����������Ҽ��˵���
    MyAction * closeWorkAction = ActionManager::instance()->crateAction(Constants::CLOSE_WORKSPACE,QIcon(""),"�رչ�����");
    ActionManager::instance()->registerAction(closeWorkAction,MyPageSwitch::instance(),SLOT(closePage()));

    MyAction * saveWorkAction = ActionManager::instance()->crateAction(Constants::SAVE_WORKSPACE,QIcon(":/images/save.png"),"����");
    ActionManager::instance()->registerAction(saveWorkAction,MyGraphicsView::instance(),SLOT(fileSave()));

    MyAction * closeLeftWorkAction = ActionManager::instance()->crateAction(Constants::CLOSE_LEFT_WORKSPACE,QIcon(""),"�ر��������");
    ActionManager::instance()->registerAction(closeLeftWorkAction,MyPageSwitch::instance(),SLOT(closeLeftPage()));

    MyAction * closeRightWorkAction = ActionManager::instance()->crateAction(Constants::CLOSE_RIGHT_WORKSPACE,QIcon(""),"�ر��Ҳ�����");
    ActionManager::instance()->registerAction(closeRightWorkAction,MyPageSwitch::instance(),SLOT(closeRightPage()));
}

//ֱ�Ӵ��������ٳ���KeyPressEvent������һ���¼��ᴦ������
void MainWindow::keyPress(QKeyEvent *event)
{
//    keyPressEvent(event);
    //�����л���������
    if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_Left)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            MyPageSwitch::instance()->switchFrontBack(true);
        }
    }
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_Right)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            MyPageSwitch::instance()->switchFrontBack(false);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //ȫ��/��ȫ��
    if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_F11)
    {
        switchFullScreen();
    }
    //�������
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_L)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            setIconWidgetState();
        }
    }
    //�����Ҳ�
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_R)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            setToolWidgetState();
        }
        else
        {
            setSimulateWidgetState();
        }
    }
    //���ع�����
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_U)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            setWorkspaceState();
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
    //����ģʽ�л�[����->��ģ]
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_M)
    {
        if(!GlobalIsSimulateState)
        {
            if(!ActionManager::instance()->action(Constants::SIMLUATE_ID)->isChecked())
            {
                ActionManager::instance()->action(Constants::SIMLUATE_ID)->setChecked(true);
            }
        }
        else
        {
            Util::showWarn("��ǰ������״̬���޷������л�!");
        }
    }
    //[��ģ->����]
    else if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_M)
    {
        if(!GlobalIsSimulateState)
        {
            if(!ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->isChecked())
            {
                ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->setChecked(true);
            }
        }
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = Util::getWarnChoice("�Ƿ��˳�����");

    if(result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
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
            setIconWidgetState();
        }
        else if(objName == QString(Constants::HIDE_TOOL_ID))
        {
            setToolWidgetState();
        }
        else if(objName == QString(Constants::HIDE_WORKSPACE_ID))
        {
            setWorkspaceState();
        }
    }
    else
    {
        if(objName == QString(Constants::HIDE_TOOL_ID))
        {
            setSimulateWidgetState();
        }
    }
}

void MainWindow::setIconWidgetState()
{
    SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setContainerVisible();
    if(SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->getContainer()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_ICON_ID)->setText("���ؿؼ���");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_ICON_ID)->setText("��ʾ�ؼ���");
    }
}

void MainWindow::setToolWidgetState(bool flag)
{
    if(flag)
    {
        SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();
    }

    if(SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->getContainer()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("�������Ա༭��");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("��ʾ���Ա༭��");
    }
}

void MainWindow::setWorkspaceState()
{
    bool isVisible = MyPageSwitch::instance()->isVisible();
    MyPageSwitch::instance()->setVisible(!isVisible);
    if(MyPageSwitch::instance()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_WORKSPACE_ID)->setText("���ع�����");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_WORKSPACE_ID)->setText("��ʾ������");
    }
}

void MainWindow::setSimulateWidgetState(bool flag)
{
    if(flag)
    {
        SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setContainerVisible();
    }

    if(SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->getContainer()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("����������");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("��ʾ������");
    }
}

//��ȡ����֧��
void MainWindow::getTecSupport()
{

}

//��ʾ����
void MainWindow::showAbout()
{
    AboutSoft dialog(this);
    dialog.exec();
//    qApp->aboutQt();
}

//�л�����ģʽ����ģ/���ݡ�
void MainWindow::switchWorkModel()
{
    QString objName = QObject::sender()->objectName();
    if(GlobalIsSimulateState)
    {
        if(objName == QString(Constants::SIMLUATE_ID))
        {
            return;
        }
        ActionManager::instance()->action(Constants::SIMLUATE_ID)->setChecked(true);

//        Util::showWarn("������δ�������޷��л�����ģ״̬!");
        return;
    }
    bool enable = false;

    MyGraphicsView::instance()->respResetSimluate();

    if(objName == QString(Constants::BUILD_MODEL_ID))
    {
        enable = true;
        GlobalWindowState = WINDOW_BUILD_MODEL;
        setWindowTitle("�����ģ������Эͬ���ù���-��ģ");
        setToolWidgetState(false);
    }
    else
    {
        GlobalWindowState = WINDOW_SIMULATE;
        setWindowTitle("�����ģ������Эͬ���ù���-����");
        setSimulateWidgetState(false);
        simulatePanel->resetSimluateFlag();
        MyGraphicsView::instance()->resetItemProcessType();
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

    MenuManager::instance()->menu(Constants::MENUBAR_FILE_MENU)->setEnabled(enable);
    MenuManager::instance()->menu(Constants::MENUBAR_EDIT_MENU)->setEnabled(enable);
    MenuManager::instance()->menu(Constants::MENUBAR_ITEM_MENU)->setEnabled(enable);

    fileBar->setEnabled(enable);
    itemBar->setEnabled(enable);
    editBar->setEnabled(enable);
    databaseBar->setEnabled(!enable);
}

//�򿪱��ر�����ļ���������ʾ�Ƿ�Ҫ���浱ǰ��ӵĿؼ�
void MainWindow::fileOpen()
{
    MY_BUILD_MODEL_ONLY

    respRestItemAction();

    MY_ASSERT(MyGraphicsView::instance()->scene())

    fileClear();

    QString openFileName = QFileDialog::getOpenFileName(this,"ѡ����ļ�","","Files(*"+SaveFileSuffix+")");
    if(!openFileName.isEmpty())
    {
        MyGraphicsView::instance()->openLocalFile(openFileName);
    }
}

//��յ�ǰ�Ŀؼ�
void MainWindow::fileClear()
{
    MY_BUILD_MODEL_ONLY

    respRestItemAction();

    if(view->getItemSize() > 1)
    {
        int result = Util::getWarnChoice("�Ƿ���ճ���������?");
        if(result == QMessageBox::Yes)
        {
            view->clearItems();
            view->respItemSizeChanged(0);
            resetEditActionState(false);
            Util::resetGlobalZValue();
        }
    }
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
    connect(MyProWizard::instance(),SIGNAL(proInfo(QString,QString)),MyGraphicsView::instance(),SLOT(addPage(QString,QString)));

    vLayout->addWidget(MyPageSwitch::instance());
    vLayout->addWidget(view);

    connect(MyPageSwitch::instance(),SIGNAL(switchPage()),MyGraphicsView::instance(),SLOT(setToolBarState()));
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

    foreach(ServiceProperty * prop,GlobalServiceProperties)
    {
        foreach (Parameter * para, prop->inputParas)
        {
            delete para;
        }
        prop->inputParas.clear();

        foreach (Parameter * para, prop->outputParas)
        {
            delete para;
        }
        prop->outputParas.clear();

        delete prop;
    }

    delete ui;
}
