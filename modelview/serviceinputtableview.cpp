#include "serviceinputtableview.h"

#include "serviceinputmodel.h"
#include "serviceinputdelegate.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QDebug>

ServiceInputTableView::ServiceInputTableView(int row, int column, QWidget *parent):
    QTableView(parent)
{
    serviceModel = new ServiceInputModel(row,column);
    serviceDelegate = new ServiceInputDelegate;

    setModel(serviceModel);
    setItemDelegate(serviceDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->setStretchLastSection(true);

    //���ñ�ͷ���
    QHeaderView * view = new QHeaderView(Qt::Horizontal,this);
    view->setResizeMode(QHeaderView::Custom);
    setHorizontalHeader(view);

    view->resizeSection(2,200);
    view->setStretchLastSection(true);

    initContextMenu();
}

//���ص�ǰ��ģ��
ServiceInputModel * ServiceInputTableView::model()const
{
    return serviceModel;
}

//��ʼ���Ҽ��˵�
void ServiceInputTableView::initContextMenu()
{
    rightMenu = new QMenu();
    insertAction = new QAction("������",rightMenu);
    deleteAction = new QAction("ɾ����",rightMenu);

    connect(insertAction,SIGNAL(triggered()),this,SLOT(insertRow()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRow()));

    rightMenu->addAction(insertAction);
    rightMenu->addAction(deleteAction);
}

//���в���
void ServiceInputTableView::insertRow()
{
    serviceModel->addRow();
}

//���в����������
void ServiceInputTableView::insertRow(Parameter * intputPara)
{
    serviceModel->addRow(intputPara);
}

//����ɾ��
void ServiceInputTableView::deleteRow()
{
    if(selectedIndexes().size() > 0)
    {
        int selectedRow = selectedIndexes().at(0).row();
        serviceModel->deleteRow(selectedRow);
    }
}

//��ձ��
void ServiceInputTableView::clearTable()
{
    serviceModel->clearTable();
}

void ServiceInputTableView::contextMenuEvent(QContextMenuEvent *)
{
//    rightMenu->exec(event->globalPos());
}

ServiceInputDelegate * ServiceInputTableView::delegate()
{
    return this->serviceDelegate;
}
