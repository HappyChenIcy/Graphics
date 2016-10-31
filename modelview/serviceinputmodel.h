/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: serviceinputmodel.h
**��  ��: wey       Version: 1.0       Date: 2016.10.09
**��  ��:
**Others: �������Ա༭ģ��
**
**�޸���ʷ:
**20161009:wey:������ʼ�汾
**
*************************************************/
#ifndef SERVICEINPUTMODEL_H
#define SERVICEINPUTMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

#include "../Header.h"

class ServiceInputModel : public QAbstractTableModel
{
public:
    ServiceInputModel(int row,int column);

    bool addRow();
    bool addRow(Parameter *intputPara);
    bool deleteRow(int rowNum);

    void setPara(ParaList list);
    ParaList getPara(){return this->paraList;}
    void clearTable();

protected:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    int row,column;

    ParaList paraList;               //��ǰ�ؼ�������
    QStringList horizonalHeadList;     //ˮƽ��ͷ
};

#endif // SERVICEINPUTMODEL_H
