/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: parameterdefinemodel.h
**��  ��: wey       Version: 1.0       Date: 2016.11.11
**��  ��: ��������ģ��
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef PARAMETERDEFINEMODEL_H
#define PARAMETERDEFINEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

#include "../Header.h"

class ParameterDefineModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ParameterDefineModel(const LoopPart loopType,int row,int column);
    ~ParameterDefineModel();

    bool addRow();
    bool addRow(VariableDefine *intputPara);
    bool addRow(ExpressDefine * intputPara);
    bool addRow(FinalExpressDefine * intputPara);
    bool deleteRow(int rowNum);
    void deleteRow(QString name);

    VariableDefineList getPara1(){return this->paraList;}
    ExpressDefineList  getPara2(){return this->expList;}
    FinalExpressDefineList getPara3(){return this->fexpList;}
    void setParameterProp(LoopProperty * prop);
    void clearTable();

signals:
    void addVariInfo(QString variName);
    void deleteVariInfo(QString variName);
    void variNumEdited();

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
    VariableDefineList paraList;        //��ǰ�ؼ�������
    ExpressDefineList expList;          //���ʽ����
    FinalExpressDefineList fexpList;    //ĩѭ���弯��

    QStringList horizonalHeadList;     //ˮƽ��ͷ

    LoopPart loopPart;          //��ǰ����ѭ���ĸ�����
};

#endif // PARAMETERDEFINEMODEL_H
