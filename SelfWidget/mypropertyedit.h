/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypropertyedit.h
**��  ��: wey       Version: 1.0       Date: 2016.10.03
**��  ��:
**Others: �ؼ����Ա༭����
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYPROPERTYEDIT_H
#define MYPROPERTYEDIT_H

#include <QDialog>

namespace Ui {
class MyPropertyEdit;
}

class MyPropertyEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPropertyEdit(QWidget *parent = 0);
    ~MyPropertyEdit();
    
private:
    Ui::MyPropertyEdit *ui;
};

#endif // MYPROPERTYEDIT_H
