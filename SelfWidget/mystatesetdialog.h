/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: MyStateSetDialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: ״̬����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSTATESETDIALOG_H
#define MYSTATESETDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateSetDialog;
}

class MyStateSetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateSetDialog(QWidget *parent = 0);
    ~MyStateSetDialog();
    
private:
    Ui::MyStateSetDialog *ui;
};

#endif // MYSTATESETDIALOG_H
