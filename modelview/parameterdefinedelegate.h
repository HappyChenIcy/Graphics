/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: parameterdefinedelegate.h
**��  ��: wey       Version: 1.0       Date: 2016.11.11
**��  ��: �����������
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef PARAMETERDEFINEDELEGATE_H
#define PARAMETERDEFINEDELEGATE_H

#include <QItemDelegate>
#include <QStringList>

#include "../Header.h"

class ParameterDefineDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ParameterDefineDelegate(LoopPart loopPart);

    void updateVariList(QStringList newList);

protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index);

private:
    QStringList  stringList;             //���������ؼ���item
    QStringList  currVariList;           //��ǰ����������ʾ�б�

    LoopPart loopPart;
};

#endif // PARAMETERDEFINEDELEGATE_H
