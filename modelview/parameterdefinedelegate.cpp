#include "parameterdefinedelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QDebug>

#include "../item/mygraphicsview.h"
#include "../item/myitem.h"
#include "../simulate/simulateutil.h"

ParameterDefineDelegate::ParameterDefineDelegate(LoopPart loopPart):
    loopPart(loopPart)
{
    switch(loopPart)
    {
        case LOOP_VARI:
                        stringList<<QString("short")<<QString("int")<<QString("long")
                                 <<QString(COMBOX_LOOP_QUOTE);
                        break;
        case LOOP_EXPRESS:
                        stringList<<">"<<">="<<"<"<<"<="<<"==";
                        break;
        case LOOP_FINAL:
                        stringList<<"++"<<"--"<<"+="<<"-=";
                        break;
    }
}

//��������Ŀؼ�
QWidget * ParameterDefineDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if(loopPart == LOOP_VARI)
    {
        if(index.column() == 0)
        {
             QComboBox * box = new QComboBox(parent);
             box->setView(new QListView);
             box->setStyleSheet(COMBOX_STYLESHEET);
//                box->setEditable(true);
             box->addItems(stringList);
             return box;
        }
        //������Ŀǰ�жϿ�����ǰ��ֵ���������⣬�˹�����ʱ����ӡ�
        else if(index.column() == 1)
        {
             //�жϵ�0�е�ֵ�����Ϊ���ã�����Ҫ��ȡ��ǰ�ؼ��ĸ�������õ�ֵ������������ʾ
             const QAbstractItemModel * pmodel = index.model();
             if(pmodel)
             {
                 QString column0Text = pmodel->index(index.row(),0).data(Qt::DisplayRole).toString();
                 if(column0Text == QString(COMBOX_LOOP_QUOTE))
                 {
                     MyItem * item =  MyGraphicsView::instance()->getTopSelectedItem();

                     if(item)
                     {
                         QList<MyItem *> pItems = SimulateUtil::instance()->getCurrParentItem(item);
                         QComboBox * box = new QComboBox(parent);
                         box->setView(new QListView);
                         box->setStyleSheet(COMBOX_STYLESHEET);

                         int index = 1;
                         foreach(MyItem * tmpItem,pItems)
                         {
                             ServiceProperty * prop = tmpItem->getServiceProp();
                             if(prop->outputParas.size() == 1)
                             {
                                 QString newItem = QString(COMBOX_START_FLAG)+QString::number(index)+"]"+tmpItem->getText()+":"+prop->outputParas.at(0)->pName;
                                 index++;
                                 box->addItem(newItem);
                             }
                         }
                         return box;
                     }
                 }
             }
        }
    }
    else if(loopPart == LOOP_EXPRESS || loopPart == LOOP_FINAL)
    {
        if(index.column() == 1 || index.column() == 0)
        {
                QComboBox * box = new QComboBox(parent);
                box->setView(new QListView);
                box->setStyleSheet(COMBOX_STYLESHEET);
//                box->setEditable(true);
                if(index.column() == 0)
                {
                    box->addItems(currVariList);
                }
                else
                {
                    box->addItems(stringList);
                }

                return box;
        }
    }
    QLineEdit * edit = new QLineEdit(parent);
    return edit;
}

//���ô���ؼ���ֵ,����б����ڵ�ǰֵ���򽫵�ǰֵ��������ѡ�б���ֹ��δѡ��ֵ���µ�ǰֵ�����ǡ�
void ParameterDefineDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::DisplayRole).toString();

    if(loopPart == LOOP_VARI)
    {
        if(index.column() == 0 )
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
//                if(!stringList.contains(value))
//                {
//                    box->insertItem(0,value);
//                    box->setCurrentIndex(0);
//                }
//                else
//                {
                    box->setCurrentIndex(stringList.indexOf(value));
//                }
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                edit->setText(value);
            }
        }
    }
    else if(loopPart == LOOP_EXPRESS || loopPart == LOOP_FINAL)
    {
        if(index.column() == 1 || index.column() == 0)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                if(index.column() == 0)
                {
                    box->setCurrentIndex(currVariList.indexOf(value));
                }
                else
                {
                    box->setCurrentIndex(stringList.indexOf(value));
                }
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                edit->setText(value);
            }
        }
    }
}

//���´���ؼ���λ�óߴ�
void ParameterDefineDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

//����ģ������
void ParameterDefineDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    QString value;

    if(loopPart == LOOP_VARI)
    {
        if(index.column() == 0)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                value = box->currentText();
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                value = edit->text();
            }
        }
    }
    else if(loopPart == LOOP_EXPRESS || loopPart == LOOP_FINAL)
    {
        if(index.column() == 1 || index.column() == 0)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                value = box->currentText();
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                value = edit->text();
            }
        }
    }

    model->setData(index,value,Qt::EditRole);
}

//������ӱ�������������ʾ�б�ֻ���Դӵ�ǰ�����б���ѡ��
void ParameterDefineDelegate::updateVariList(QStringList newList)
{
    currVariList = newList;
}
