/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mystatemodeldialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��:ģ�Ϳ�״̬����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSTATEMODELDIALOG_H
#define MYSTATEMODELDIALOG_H

#include <QDialog>
class MyChooseBar;

#include "../Header.h"

namespace Ui {
class MyStateModelDialog;
}

class MyStateModelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateModelDialog(QWidget *parent = 0);
    ~MyStateModelDialog();

    void setModelProp(StateModelProperty &);
    StateModelProperty & getModelProp(){return this->prop;}
    
private slots:
    void respContinueAction();
    void respAddItem();
    void updateInfo();

private:
    Ui::MyStateModelDialog *ui;
    MyChooseBar * chooseBar;

    StateModelProperty prop;

    QList<StatInnerProperty> props;
    QString continueContent;
};

#endif // MYSTATEMODELDIALOG_H
