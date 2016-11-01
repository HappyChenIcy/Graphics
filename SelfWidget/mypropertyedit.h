/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypropertyedit.h
**��  ��: wey       Version: 1.0       Date: 2016.10.03
**��  ��:
**Others: �ؼ����Ա༭����
**
**�޸���ʷ:
**20160120:wey:�޸ķ����������Ϣ�����Ӵ����ݿ��ȡ��Ϣ
*************************************************/
#ifndef MYPROPERTYEDIT_H
#define MYPROPERTYEDIT_H

#include <QDialog>

class ServiceInputTableView;
class MyItem;
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

    void initProp(ServiceProperty * prop);
    void updateDelegateList(QList<MyItem *> pItems);

private slots:
    void confirmPropety();
    void cancelProperty();
    void switchServiceInfo(int index);
    
private:
    void initServiceData();

    Ui::MyPropertyEdit *ui;

    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    ServiceProperty * currItemProp;

    bool isComboxAutoChanged;               //����������Զ��ı䣬��������Ӧ�ۺ���
};

#endif // MYPROPERTYEDIT_H
