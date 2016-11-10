#include "myprowizard.h"

#include "mywelcomepage.h"
#include "myfinaldescpage.h"
#include "../Header.h"
#include "../global.h"
#include "../mainwindow.h"

#include <QDebug>

MyProWizard * MyProWizard::wizard = NULL;

MyProWizard * MyProWizard::instance()
{
    if(!wizard)
    {
        wizard = new MyProWizard(GlobalMainWindow);
    }
    return wizard;
}

MyProWizard::MyProWizard(QWidget *parent):
    QWizard(parent)
{
    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    setWindowTitle("�����������");
    QIcon icon(":/images/softicon.png");
    setPixmap(QWizard::LogoPixmap,icon.pixmap(32));
    setWizardStyle(QWizard::AeroStyle);
    createSubPage();
}

void MyProWizard::createSubPage()
{
    welcomePage = new MyWelcomePage;
    finalDescPage = new MyFinalDescPage;

    setPage(WELCOME_PAGE,welcomePage);
    setPage(FINALDESC_PAGE,finalDescPage);
}

//��ȡ��д����Ϣ
void MyProWizard::accept()
{
    emit proInfo(field("proPath").toString(),field("proName").toString());
    QDialog::accept();
}
