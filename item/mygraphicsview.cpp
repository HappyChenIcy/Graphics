#include "mygraphicsview.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDataStream>
#include <QDebug>

#include "myscene.h"
#include "../SelfWidget/nodeeditdialog.h"
#include "../Header.h"
#include "../global.h"

MyGraphicsView * MyGraphicsView::viewManager = NULL;

MyGraphicsView * MyGraphicsView::instance()
{
   return viewManager;
}

MyGraphicsView::MyGraphicsView(QWidget * parent):
    QGraphicsView(parent)
{
    viewManager = this;

    nodeEdit = NULL;

    setAcceptDrops(true);

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
            event->ignore();
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

void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
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

        }
        else
        {
           MyScene * tmpScene = dynamic_cast<MyScene *>(scene());
           if(tmpScene)
           {
               tmpScene->addItem((GraphicsType)graphicsType,mapToScene(event->pos()));
           }
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

MyGraphicsView::~MyGraphicsView()
{
    if(nodeEdit)
    {
        delete nodeEdit;
        nodeEdit = NULL;
    }
}
