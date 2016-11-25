/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypropertyedit.h
**��  ��: wey       Version: 1.0       Date: 2016.10.03
**��  ��:
**Others: �ؼ����Ա༭����
**
**�޸���ʷ:
**20160120:wey:�޸ķ����������Ϣ�����Ӵ����ݿ��ȡ��Ϣ
**20161123:wey:��������������һ������������ö������е�һ��
**20161125:wey:������������Զ�������֧�ֶ�����������
*************************************************/
#ifndef MYPROPERTYEDIT_H
#define MYPROPERTYEDIT_H

#include <QDialog>

class ServiceInputTableView;
class MyItem;
class MyChooseBar;
struct ServiceProperty;

namespace Ui {
class MyPropertyEdit;
}

class MyPropertyEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPropertyEdit(QWidget *parent = 0);
    ~MyPropertyEdit();

    void initProp(ServiceProperty * prop, bool isEditable = true);
    void updateDelegateList(QList<MyItem *> pItems);

private slots:
    void confirmPropety();
    void switchServiceInfo(int index);
    
private:
    void initServiceData();
    void updateLeftQuoteTable(QList<MyItem *> & pItems);

    Ui::MyPropertyEdit *ui;

    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    ServiceProperty * currItemProp;

    bool isComboxAutoChanged;               //����������Զ��ı䣬��������Ӧ�ۺ���
    MyChooseBar * chooseBar;
    bool isEditState;                       //�Ƿ�Ϊ�༭״̬
};

#endif // MYPROPERTYEDIT_H
