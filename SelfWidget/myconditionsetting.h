/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myconditionsetting.h
**��  ��: wey       Version: 1.0       Date: 2016.10.28
**��  ��:
**Others:���������
**
**�޸���ʷ:
**20161121:wey:�����������ʽת��������ʾ
*************************************************/
#ifndef MYCONDITIONSETTING_H
#define MYCONDITIONSETTING_H

#include <QDialog>

class MyChooseBar;
class QListWidgetItem;
class MyItem;
class MyPlainTextEdit;
struct JudgeProperty;

namespace Ui {
class MyConditionSetting;
}

class MyConditionSetting : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyConditionSetting(QWidget *parent = 0);
    ~MyConditionSetting();

    void setJudgeProp(MyItem *item);

private slots:
    void addQuote(QListWidgetItem *item);
    void respConfirmPressed();
    
private:
    Ui::MyConditionSetting *ui;

    MyPlainTextEdit * plainEdit;
    MyChooseBar * chooseBar;
    JudgeProperty * jprop;
};

#endif // MYCONDITIONSETTING_H
