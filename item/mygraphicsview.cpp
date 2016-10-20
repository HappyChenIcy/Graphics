#include "mygraphicsview.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDataStream>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>

#include "myscene.h"
#include "../SelfWidget/nodeeditdialog.h"
#include "../SelfWidget/mytextinput.h"
#include "../SelfWidget/mypropertyedit.h"
#include "../Header.h"
#include "../global.h"
#include "../mainwindow.h"
#include "../manager/actionmanager.h"
#include "../manager/mylinecombobox.h"
#include "../Constants.h"
#include "mytextitem.h"
#include "myarrow.h"
#include "myitem.h"
#include "mynodeport.h"
#include "rotateline.h"

using namespace Graphics;

MyGraphicsView * MyGraphicsView::viewManager = NULL;

MyGraphicsView * MyGraphicsView::instance()
{
   return viewManager;
}

MyGraphicsView::MyGraphicsView(MainWindow * parent):
    parentWindow(parent),
    QGraphicsView(parent)
{
    viewManager = this;

    nodeEdit = NULL;
    isMoving = false;
    isCtrlPressed = false;
    viewIsDragable = true;
    lockState = ITEM_ALL_UNLOCK;
    sceneScaleFactor = 100;

    setAcceptDrops(true);
    initView();
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

MyScene * MyGraphicsView::scene()
{
    return this->myScene;
}

void MyGraphicsView::initView()
{
    SceneWidth = SceneHeight = 5000;

    rightMenu = new QMenu;
    viewRightMenu = new QMenu;
}

//�����µ�scene
MyScene * MyGraphicsView::addScene(QString id)
{
    MyScene  * tmpScene = new MyScene(rightMenu);
    tmpScene->setSceneRect(0,0,SceneWidth,SceneHeight);
    tmpScene->setId(id);

    showScene(tmpScene);

    connect(tmpScene,SIGNAL(resetItemAction()),parentWindow,SLOT(respRestItemAction()));
    connect(tmpScene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));
    connect(tmpScene,SIGNAL(deleteKeyPress()),this,SLOT(deleteItem()));
    connect(tmpScene,SIGNAL(ctrlLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(tmpScene,SIGNAL(ctrlUnLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(tmpScene,SIGNAL(itemSizeChanged(int)),parentWindow,SLOT(respItemSizeChanged(int)));
    connect(tmpScene,SIGNAL(itemPropChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
    connect(tmpScene,SIGNAL(editCurrItem()),this,SLOT(editTextItem()));
    connect(tmpScene,SIGNAL(ctrlPropEditKeyPress()),this,SLOT(editPropertyItem()));

    return tmpScene;
}

//���õ�ǰ�ĳ���
void MyGraphicsView::showScene(MyScene *scene)
{
    myScene = scene;
    MyGraphicsView::setScene(myScene);
}

//��ʼ���Ҽ��˵�
void MyGraphicsView::addContextMenuItem()
{
    rightMenu->clear();
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
    {
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
        rightMenu->addSeparator();
        rightMenu->addAction(ActionManager::instance()->action(Constants::LOCK_ID));
        rightMenu->addAction(ActionManager::instance()->action(Constants::UNLOCK_ID));
        rightMenu->addSeparator();
        rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));
    }
    else if(GlobalWindowState == WINDOW_SIMULATE)
    {
        rightMenu->addAction(ActionManager::instance()->action(Constants::PROPERTY_EDIT_ID));
    }
}

//������ͼ���Ҽ��˵�
void MyGraphicsView::addViewContextMenu()
{
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::UNDO_ID));
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::REDO_ID));
    viewRightMenu->addSeparator();
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::CLEAR_PASTE_ID));
    viewRightMenu->addSeparator();
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::DRAG_ABLE_ID));
}

//����view�Ƿ������ק
void MyGraphicsView::setViewDragEnable(bool enable)
{
    viewIsDragable = enable;
    if(enable)
    {
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setIcon(QIcon(":/images/dragable.png"));
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setText("����������ק");
    }
    else
    {
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setIcon(QIcon(":/images/drageunable.png"));
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setText("���ڽ�ֹ��ק");
    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        isCtrlPressed = true;
    }

    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        isCtrlPressed = false;
    }

    QGraphicsView::keyReleaseEvent(event);
}

//�������¼�����ϼ���Ctrl�¼�������ͼ������
void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(isCtrlPressed)
    {
        int direct = event->delta();
        if(direct > 0)
        {
            emit zoomIn();
        }
        else if(direct < 0)
        {
            emit zoomOut();
        }
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    MY_ASSERT(myScene)
    if(viewIsDragable && myScene->selectedItems().size()==0 )
    {
        pressPoint = event->pos();

        isMoving = true;
        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    MY_ASSERT(myScene)
    if(viewIsDragable && myScene->selectedItems().size() == 0 && isMoving &&CurrAddGraType == GRA_NONE)
    {
        movePoint = event->pos();

        QPoint tmpPoint = movePoint - pressPoint;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-tmpPoint.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()-tmpPoint.y());

        pressPoint = movePoint;
    }

    QGraphicsView::mouseMoveEvent(event);
}

//�Ƚ���ϵͳ�����ڿؼ�������ͷ�ʱ���ؼ��Ȳ���
void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    isMoving = false;
    setCursor(Qt::ArrowCursor);
}

//�Ҽ��˵��¼������ж�scene��ǰ�������Ƿ����item�����û��view����Ӧ��
void MyGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    MY_ASSERT(myScene)
    QPointF scenePos = mapToScene(event->pos().x(),event->pos().y());
    int curItemSize = myScene->items(scenePos).size();

    QGraphicsView::contextMenuEvent(event);

    if(curItemSize == 0)
    {
        viewRightMenu->exec(mapToGlobal(event->pos()));
    }
}

//���ؼ����룬����������ͽ����ж�
void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//����������

        int graphicsType;
        stream>>graphicsType;
        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT)
        {
            //�˴������ϣ��GraphicsView�����¼�����ô��Ҫ�����¼�����ϵͳ��������ֱ�Ӻ��ԡ�1��
            QGraphicsView::dragEnterEvent(event);
        }
        else
        {
           event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

//��ִ�������ƶ�ʱ���Ƚ��¼����ɸ��ദ������ʱ����Ա������ؼ�����
void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    if(event->mimeData()->hasFormat("MyItem"))
    {
        event->acceptProposedAction();
    }
}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    MY_ASSERT(myScene)
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//����������

        int graphicsType;
        stream>>graphicsType;

        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT)
        {
            QGraphicsView::dropEvent(event);
        }
        else
        {
            myScene->clearSelection();
            myScene->addItem((GraphicsType)graphicsType,mapToScene(event->pos()));
        }
    }
}

//��ʾ�˿ڱ༭����
void MyGraphicsView::showNodePortEdit(MyNodePort *nodePort)
{
    if(!nodeEdit)
    {
        nodeEdit = new NodeEditDialog(this);
    }
    nodeEdit->initNodeInfo(nodePort);
    nodeEdit->exec();
}

//������ǰ��
void MyGraphicsView::undoAndRedoItem()
{

}

//Ŀǰ֧��һ���ؼ��ļ��У�ֻ֧�ֳ���ͷ����ؼ�����
void MyGraphicsView::cutItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(myScene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            foreach (MyNodePort * node, item->getNodePorts())
            {
                NodePortProperty  props;
                props.direct = node->getDragDirect();
                props.scaleFactor = node->getScaleFactor();
                cutTmpInfo.nodeProperties.push_back(props);
            }
            deleteItem();
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            delete item;
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
    }
}

//����Item��֧�ֶ�����ʽ�ġ��˿ڵĸ���
void MyGraphicsView::copyItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(myScene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            cutTmpInfo.nodeProperties.clear();

            foreach (MyNodePort * node, item->getNodePorts())
            {
                NodePortProperty  props;
                props.direct = node->getDragDirect();
                props.scaleFactor = node->getScaleFactor();
                cutTmpInfo.nodeProperties.push_back(props);
            }
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->toPlainText();
            cutTmpInfo.nodeProperties.clear();
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
    }
}

//ճ��
void MyGraphicsView::pasteItem()
{
    MY_ASSERT(myScene)
    myScene->addItem(cutTmpInfo,true);
}

//��ռ��а�
void MyGraphicsView::clearPasteItem()
{
    ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(false);
}

//����Ҫ��scene��ɾ��item
void MyGraphicsView::deleteItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    foreach(QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * tmp = dynamic_cast<MyArrow *>(item);

            tmp->getStartItem()->removeArrow(tmp);
            tmp->getEndItem()->removeArrow(tmp);
            myScene->removeItem(tmp);

            delete tmp;
        }
    }

    selectedItems = myScene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * tmp = dynamic_cast<MyItem *>(item);
            tmp->removeDragLineArrows();
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    selectedItems = myScene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * tmp = dynamic_cast<MyTextItem *>(item);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    selectedItems = myScene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * tmp = dynamic_cast<MyNodePort *>(item);
            tmp->removeArrows();
            //�Ӹ��༯����ɾ��
            dynamic_cast<MyItem *>(tmp->getParentItem())->removeNodePort(tmp);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    myScene->update();
}

//������ת
void MyGraphicsView::rotateItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() !=  1)
    {
        return;
    }

    QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

    if(itemName == TYPE_ID(MyItem))
    {
        MyItem * graphicsTmp = dynamic_cast<MyItem *>(myScene->selectedItems().first());

        QString objName = QObject::sender()->objectName();
        if(objName == QString(Constants::ROTATE_LEFT_ID))
        {
            graphicsTmp->updateRotation(270);
        }
        else if(objName == QString(Constants::ROTATE_RIGHT_ID))
        {
            graphicsTmp->updateRotation(90);
        }
        graphicsTmp->setRotation(graphicsTmp->getProperty().rotateDegree);
    }
    else
    {

    }
}

//�ö������ڵײ�
void MyGraphicsView::bringZItem()
{
    MY_ASSERT(myScene)
    if (myScene->selectedItems().isEmpty())
    {
         return;
    }

    QGraphicsItem *selectedItem = myScene->selectedItems().first();
//    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();     //�ҵ��뵱ǰitem���غϵ�items
    QList<QGraphicsItem * > allItems = myScene->items();

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
    QString itemName = TYPE_ID(*selectedItem);
    if(itemName == TYPE_ID(MyItem))
    {
        MyItem * tmp = dynamic_cast<MyItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
    else if(itemName == TYPE_ID(MyTextItem))
    {
        MyTextItem * tmp = dynamic_cast<MyTextItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
}

//���̿�ݼ�Ctrl+L/U(��������/����)
void MyGraphicsView::respCtrlLockKeyPress()
{
    QString objName = QObject::sender()->objectName();

    getSelectedLockState();

    if(objName == QString(Constants::LOCK_ID))
    {
        if(lockState == ITEM_NOT_ALL_LOCK || lockState == ITEM_ALL_UNLOCK)
        {
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
            setSelectedItemLockState(true);
        }
    }
    else if(objName == QString(Constants::UNLOCK_ID))
    {
        if(lockState == ITEM_ALL_LOCK)
        {
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
            setSelectedItemLockState(false);
        }
    }
}

//���������
void MyGraphicsView::lockAndunlockItem()
{
    QString objName = QObject::sender()->objectName();

    bool moveable = false;

    if(objName == QString(Constants::LOCK_ID))
    {
        moveable = false;
    }
    else if(objName == QString(Constants::UNLOCK_ID))
    {
        moveable = true;
    }

    ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(moveable);
    ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(!moveable);

    setSelectedItemLockState(moveable);
}

//����ѡ�пؼ�����״̬
void MyGraphicsView::setSelectedItemLockState(bool flag)
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() > 0)
    {
        foreach (QGraphicsItem * item, selectedItems)
        {
            QString itemName = TYPE_ID(*item);
            if(itemName == TYPE_ID(MyItem))
            {
                MyItem * tmp = dynamic_cast<MyItem*>(item);
                tmp->setMoveable(flag);
            }
            else if(itemName == TYPE_ID(MyTextItem))
            {
                MyTextItem * tmp = dynamic_cast<MyTextItem*>(item);
                tmp->setMoveable(flag);
            }
        }
        myScene->update();
    }
}

//����item������
void MyGraphicsView::respPropertyUpdate(ItemProperty property)
{
    MY_ASSERT(myScene)
    //��ѡ����ת��󣬻�ͬʱѡ���������item
    if(myScene->selectedItems().size() == 1 || myScene->selectedItems().size() ==2)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        if((itemName == TYPE_ID(MyItem)) || (itemName == TYPE_ID(RotateLine)))
        {
            MyItem * myItem = NULL;
            foreach (QGraphicsItem * selectItem, myScene->selectedItems())
            {
                if(TYPE_ID(MyItem) == TYPE_ID(*selectItem))
                {
                     myItem = dynamic_cast<MyItem *>(selectItem);
                     break;
                }
            }
            if(property.isMoveable)
            {
                MY_ASSERT(myItem);
                myItem->setProperty(property);
            }
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(myScene->selectedItems().first());
            textItem->setProperty(property);
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());
            arrowItem->setProperty(property);
        }

        myScene->update();
    }
}

//�л���ͼ����
void MyGraphicsView::sceneScaled(int currScale)
{
     double newScale = currScale/100.0;
     sceneScaleFactor = currScale;
     QMatrix oldMatrix = matrix();
     resetMatrix();
     translate(oldMatrix.dx(), oldMatrix.dy());
     scale(newScale, newScale);
}

//��ѡ���item״̬�ı�󣬸���action
void MyGraphicsView::updateActions()
{
    MY_ASSERT(myScene)
    int selectedSize = myScene->selectedItems().size();

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
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(false);
//        ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(false);
//        ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(false);

    }
    else if(selectedSize == 1)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(true);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);
        ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(false);

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(myScene->selectedItems().first());
            property = myItem->getProperty();
            getSelectedLockState();
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(myScene->selectedItems().first());
            property = textItem->getProperty();

            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());
            property = arrowItem->getProperty();
            AddLineType startType = arrowItem->getStartLineType();
            AddLineType endType = arrowItem->getEndLineType();

            ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

            ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);

            ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->updateIndex((int)startType);
            ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->updateIndex((int)endType);

            ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(true);
            ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(true);
        }
        else if(itemName == TYPE_ID(MyNodePort))
        {
            ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

            ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
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
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        //��ȡ����״̬
        getSelectedLockState();
    }

    myScene->update();

    emit initToolBox(selectedSize,property);
}

//��ȡѡ���item������״̬
void MyGraphicsView::getSelectedLockState()
{
    MY_ASSERT(myScene)
    int myItemNum = 0;
    int myItemLockNum = 0;
    int myItemUnLockNum = 0;

    foreach (QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyItem))
        {
            myItemNum ++;

            MyItem * myItem = dynamic_cast<MyItem *>(item);
            if(myItem->isMoveable())
            {
                myItemUnLockNum++;
            }
            else
            {
                myItemLockNum++;
            }
        }
    }

    //ȫ��û����
    if(myItemLockNum == myItemNum)
    {
        lockState = ITEM_ALL_UNLOCK;
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
    }
    //ȫ������
    else if(myItemUnLockNum == myItemNum)
    {
        lockState = ITEM_ALL_LOCK;
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
    }
    //����һ����
    else
    {
        lockState = ITEM_NOT_ALL_LOCK;
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
    }
}

//����ѡ����������ʽ
void MyGraphicsView::setSelectedLineType(int type)
{
    MY_ASSERT(myScene)
    if(myScene->selectedItems().size() == 1)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));
        if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());

            QString objName = QObject::sender()->objectName();
            if(objName == QString(Constants::LEFT_LINE_ID))
            {
                arrowItem->setStartLineType(type);
            }
            else if(objName == QString(Constants::RIGHT_LINE_ID))
            {
                arrowItem->setEndLineType(type);
            }
        }
    }
}

//�༭�ı�
void MyGraphicsView::editTextItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();
    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem*>(selectedItems.first());

            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());

            //���޸����ֺ���Ҫ���½���Ϣ�������Ҳ�Ĺ�������
            emit initToolBox(selectedItems.size(),item->getProperty());
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * item = dynamic_cast<MyArrow*>(selectedItems.first());

            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());
        }
    }
}

//�༭�ؼ�������
void MyGraphicsView::editPropertyItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();
    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem*>(selectedItems.first());
            MyPropertyEdit propertyEdit(this);
            propertyEdit.initProp(item->getServiceProp());

            propertyEdit.exec();
        }
    }
}

//��ȡ��ǰģ�������ڵ�item����
int MyGraphicsView::getItemSize()
{
     if(myScene)
     {
         return myScene->items().size();
     }
     return 0;
}

//������е�item
void MyGraphicsView::clearItems()
{
    MY_ASSERT(myScene)
    myScene->clear();
}

//��ɾ�����һ���������󣬽�scene��Ϊ�գ����ж�
void MyGraphicsView::deleteScene()
{
    myScene = NULL;
}

//��ȡˮƽ��ͼ��λ��
qreal MyGraphicsView::getHorizonalValue()
{
    return horizontalScrollBar()->value();
}

//��ȡˮƽ��ͼ��λ��
qreal MyGraphicsView::getVertiaclValue()
{
    return verticalScrollBar()->value();
}

//��ȡˮƽ��ͼ��λ��
qreal MyGraphicsView::getScaleValue()
{
    return sceneScaleFactor;
}

//�������л������ղ�ͬ�������������ű���
void MyGraphicsView::setScaleValue(int value)
{
    sceneScaled(value);
    emit scaleValue(value);
}

//�ƶ���ͼ
void MyGraphicsView::transformView(qreal hValue, qreal value)
{
    horizontalScrollBar()->setValue(hValue);
    verticalScrollBar()->setValue(value);
}

//ģ�⿪ʼǰ�����ϴ�ģ���״̬���
void MyGraphicsView::respResetSimluate()
{
    if(myScene)
    {
        foreach(QGraphicsItem * item,myScene->items())
        {
            if(TYPE_ID(*item) == TYPE_ID(MyItem))
            {
                MyItem * tmp = dynamic_cast<MyItem *>(item);
                if(tmp && tmp->isHightShow())
                {
                    tmp->hightLightItem(false);
                }
            }
        }
    }
}

MyGraphicsView::~MyGraphicsView()
{
    if(nodeEdit)
    {
        delete nodeEdit;
        nodeEdit = NULL;
    }

    if(myScene)
    {
        delete myScene;
        myScene = NULL;
    }
}
