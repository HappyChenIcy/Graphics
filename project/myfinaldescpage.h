/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myfinaldescpage.h
**��  ��: wey       Version: 1.0       Date: 2016.11.09
**��  ��: �������̻�����ʾҳ
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYFINALDESCPAGE_H
#define MYFINALDESCPAGE_H

#include <QWizardPage>

namespace Ui {
class MyFinalDescPage;
}

class MyFinalDescPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MyFinalDescPage(QWidget *parent = 0);
    ~MyFinalDescPage();

protected:
    void initializePage();
    
private:
    Ui::MyFinalDescPage *ui;
};

#endif // MYFINALDESCPAGE_H
