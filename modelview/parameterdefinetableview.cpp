#include "parameterdefinetableview.h"

#include "parameterdefinemodel.h"
#include "parameterdefinedelegate.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QDebug>

ParameterDefineTableView::ParameterDefineTableView(LoopPart loopPart, int row, int column, QWidget *parent):
    QTableView(parent)
{
    paraModel = new ParameterDefineModel(loopPart,row,column);
    paraDelegate = new ParameterDefineDelegate(loopPart);

    connect(paraModel,SIGNAL(variNumEdited()),this,SIGNAL(variContentEdited()));
    connect(paraModel,SIGNAL(deleteVariInfo(QString)),this,SIGNAL(deleteVari(QString)));

    setModel(paraModel);
    setItemDelegate(paraDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->setStretchLastSection(true);

    //���ñ�ͷ���
    QHeaderView * view = new QHeaderView(Qt::Horizontal,this);
    view->setResizeMode(QHeaderView::Custom);
    setHorizontalHeader(view);

    view->setStretchLastSection(true);

    if(loopPart == LOOP_VARI)
    {
        view->resizeSection(1,200);
    }
    else
    {
        view->resizeSection(0,200);
    }

    initContextMenu();
}

//��ʼ���Ҽ��˵�
void ParameterDefineTableView::initContextMenu()
{
    rightMenu = new QMenu();
    insertAction = new QAction("������",rightMenu);
    deleteAction = new QAction("ɾ����",rightMenu);

    connect(insertAction,SIGNAL(triggered()),this,SLOT(insertRow()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRow()));

    rightMenu->addAction(insertAction);
    rightMenu->addAction(deleteAction);
}

void ParameterDefineTableView::contextMenuEvent(QContextMenuEvent *event)
{
    rightMenu->exec(event->globalPos());
}

//���в���
void ParameterDefineTableView::insertRow()
{
    paraModel->addRow();
}

//���в����������
//void ParameterDefineTableView::insertRow(Parameter * intputPara)
//{
//    serviceModel->addRow(intputPara);
//}

//����ɾ��
void ParameterDefineTableView::deleteRow()
{
    if(selectedIndexes().size() > 0)
    {
        int selectedRow = selectedIndexes().at(0).row();
        paraModel->deleteRow(selectedRow);
    }
}

void ParameterDefineTableView::setLoopItemProp(LoopProperty *prop)
{
    paraModel->setParameterProp(prop);
}

//���ص�ǰ��ģ��
ParameterDefineModel * ParameterDefineTableView::model()const
{
    return paraModel;
}

//���ص�ǰ��ģ��
ParameterDefineDelegate * ParameterDefineTableView::delegate()
{
    return paraDelegate;
}

ParameterDefineTableView::~ParameterDefineTableView()
{
    delete paraDelegate;
    delete paraModel;
}
