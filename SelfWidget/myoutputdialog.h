/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myoutputdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.30
**��  ��: ���������
**Others:������ʾ�������Ϣ����������ǰ��Ĳ���
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYOUTPUTDIALOG_H
#define MYOUTPUTDIALOG_H

#include <QDialog>

class MyChooseBar;
class MyPlainTextEdit;
class MyItem;
class QListWidgetItem;
struct JudgeProperty;

namespace Ui {
class MyOutputDialog;
}

class MyOutputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyOutputDialog(QWidget *parent = 0);
    ~MyOutputDialog();

    void setOutput(MyItem * item, bool isEditable = true);

private slots:
    void respButtYesPress();
    void addQuote(QListWidgetItem* item);
    
private:
    Ui::MyOutputDialog *ui;

    MyPlainTextEdit * plainEdit;
    MyChooseBar * chooseBar;

    JudgeProperty * jprop;

    bool isEditState;               //�Ƿ�Ϊ�ɱ༭״̬
};

#endif // MYOUTPUTDIALOG_H
