/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myportoutputdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: ����˿�����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYPORTOUTPUTDIALOG_H
#define MYPORTOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class MyPortOutputDialog;
}

class MyPortOutputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortOutputDialog(QWidget *parent = 0);
    ~MyPortOutputDialog();
    
private:
    Ui::MyPortOutputDialog *ui;
};

#endif // MYPORTOUTPUTDIALOG_H
