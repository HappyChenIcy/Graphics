/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mystateinternaleventdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: ״̬�ڲ��¼�����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSTATEINTERNALEVENTDIALOG_H
#define MYSTATEINTERNALEVENTDIALOG_H

#include <QDialog>

#include "Header.h"

class MyChooseBar;

namespace Ui {
class MyStateInternalEventDialog;
}

class MyStateInternalEventDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateInternalEventDialog(QWidget *parent = 0);
    ~MyStateInternalEventDialog();

    bool getResult(StatInnerProperty & property);
    void showDesc(StatInnerProperty property);

private slots:
    void respConfirm();
    void respCancel();
    
private:
    Ui::MyStateInternalEventDialog *ui;

    MyChooseBar * chooseBar;
    StatInnerProperty  prop;
    bool isOkPressed;
};

#endif // MYSTATEINTERNALEVENTDIALOG_H
