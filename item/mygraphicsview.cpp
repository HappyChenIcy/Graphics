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
#include "../Header.h"
#include "../global.h"
#include "../mainwindow.h"
#include "../actionmanager.h"
#include "../Constants.h"
#include "mytextitem.h"
#include "myarrow.h"
#include "myitem.h"
#include "mynodeport.h"

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

    myScene = new MyScene(rightMenu);
    myScene->setSceneRect(0,0,SceneWidth,SceneHeight);
    setScene(myScene);

    connect(myScene,SIGNAL(resetItemAction()),parentWindow,SLOT(respRestItemAction()));
    connect(myScene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));
    connect(myScene,SIGNAL(deleteKeyPress()),this,SLOT(deleteItem()));
    connect(myScene,SIGNAL(ctrlLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(myScene,SIGNAL(ctrlUnLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(myScene,SIGNAL(itemSizeChanged(int)),parentWindow,SLOT(respItemSizeChanged(int)));
    connect(myScene,SIGNAL(itemPropChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
}

//��ʼ���Ҽ��˵�
void MyGraphicsView::addContextMenuItem()
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
    if(viewIsDragable)
    {
        pressPoint = event->pos();

        isMoving = true;

        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(viewIsDragable && scene()->selectedItems().size() == 0 && isMoving &&CurrAddGraType == GRA_NONE)
    {
        movePoint = event->pos();

        QPoint tmpPoint = movePoint - pressPoint;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-tmpPoint.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()-tmpPoint.y());

        pressPoint = movePoint;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    isMoving = false;
    setCursor(Qt::ArrowCursor);
    QGraphicsView::mouseReleaseEvent(event);
}

//�Ҽ��˵��¼������ж�scene��ǰ�������Ƿ����item�����û��view����Ӧ��
void MyGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
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

//���
void MyGraphicsView::pasteItem()
{
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
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    foreach(QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * tmp = dynamic_cast<MyArrow *>(item);

            if(tmp->getLineType() == LINE_MYITEM)
            {
                tmp->getStartItem()->removeArrow(tmp);
                tmp->getEndItem()->removeArrow(tmp);
            }
            else if(tmp->getLineType() == LINE_NODEPORT)
            {
                tmp->getStartNodePort()->removeArrow(tmp);
                tmp->getEndNodePort()->removeArrow(tmp);
            }
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
            tmp->removeArrows();
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
            tmp->getParentItem()->removeNodePort(tmp);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    myScene->update();
}

//������ת
void MyGraphicsView::rotateItem()
{
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
            graphicsTmp->updateRotation(-90);
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
    if(myScene->selectedItems().size() == 1)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(myScene->selectedItems().first());
            if(property.isMoveable)
            {
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
     QMatrix oldMatrix = matrix();
     resetMatrix();
     translate(oldMatrix.dx(), oldMatrix.dy());
     scale(newScale, newScale);
}

//��ѡ���item״̬�ı�󣬸���action
void MyGraphicsView::updateActions()
{
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
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(myScene->selectedItems().first());
            property = myItem->getProperty();

//            bool lock = myItem->isMoveable();
//            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(lock);
//            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(!lock);
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
        else if(itemName == TYPE_ID(MyNodePort))
        {
//            MyNodePort  * nodePort = dynamic_cast<MyNodePort *>(myScene->selectedItems().first());
//            property = nodePort->getProperty();
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

//�༭�ı�
void MyGraphicsView::editTextItem()
{
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
    }
}

//������е�item
void MyGraphicsView::clearItems()
{
    myScene->clear();
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
