/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myloopdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.11
**��  ��: ѭ������������
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYLOOPDIALOG_H
#define MYLOOPDIALOG_H

#include <QDialog>

class ParameterDefineTableView;
class MyItem;
class MyChooseBar;
struct LoopProperty;

namespace Ui {
class MyLoopDialog;
}

class MyLoopDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyLoopDialog(QWidget *parent = 0);
    ~MyLoopDialog();

    void setLoopItemProp(LoopProperty * prop);

private slots:
    void respButtYesPress();
    void respVariEdited();
    void respDeleteVari(QString name);
    
private:
    Ui::MyLoopDialog *ui;

    ParameterDefineTableView * express1View;
    ParameterDefineTableView * express2View;
    ParameterDefineTableView * express3View;

    LoopProperty * loopProp;
    MyChooseBar * chooseBar;
};

#endif // MYLOOPDIALOG_H
