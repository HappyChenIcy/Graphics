/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mywelcomepage.h
**��  ��: wey       Version: 1.0       Date: 2016.11.09
**��  ��: ��ӭҳ��
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYWELCOMEPAGE_H
#define MYWELCOMEPAGE_H

#include <QWizardPage>

namespace Ui {
class MyWelcomePage;
}

class MyWelcomePage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MyWelcomePage(QWidget *parent = 0);
    ~MyWelcomePage();

protected:
    bool isComplete() const;
    
private slots:
    void showFilePathSelect();
    void validatePro(QString);

private:
    void showError(QString text);

    Ui::MyWelcomePage *ui;

    bool isInfoFilled;              //��Ϣ�Ƿ���д����
};

#endif // MYWELCOMEPAGE_H
