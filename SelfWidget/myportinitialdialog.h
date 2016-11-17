/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myportinitialdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: ��ʼ���˿����ô���
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYPORTINITIALDIALOG_H
#define MYPORTINITIALDIALOG_H

#include <QDialog>

namespace Ui {
class MyPortInitialDialog;
}

class MyChooseBar;

#include "../Header.h"

class MyPortInitialDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortInitialDialog(QWidget *parent = 0);
    ~MyPortInitialDialog();

    void setProp(StatePortProperty & prop);
    StatePortProperty & getProp();

private slots:
    void updateProp();
    
private:
    Ui::MyPortInitialDialog *ui;

    StatePortProperty sprop;
    MyChooseBar * chooseBar;
};

#endif // MYPORTINITIALDIALOG_H
