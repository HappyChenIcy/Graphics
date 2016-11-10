#include "mywelcomepage.h"
#include "ui_mywelcomepage.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

MyWelcomePage::MyWelcomePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MyWelcomePage)
{
    ui->setupUi(this);

    setTitle("��Ŀ���ܺ�λ��");
    setSubTitle("This wizard generates a complex model project.");
    setWindowIcon(QIcon(":/images/softicon.png"));

    ui->error->hide();
    ui->proPath->setEnabled(false);
    isInfoFilled = false;

    connect(ui->viewPath,SIGNAL(clicked()),this,SLOT(showFilePathSelect()));
    connect(ui->proName,SIGNAL(textChanged(QString)),this,SLOT(validatePro(QString)));
    connect(ui->proPath,SIGNAL(textChanged(QString)),this,SLOT(validatePro(QString)));

    registerField("proName",ui->proName);
    registerField("proPath",ui->proPath);
}

//���ڿ�����һҳ�Ƿ����
bool MyWelcomePage::isComplete()const
{
    return isInfoFilled;
}

//��֤��������·���Ƿ���ȷ
void MyWelcomePage::validatePro(QString)
{
    QString proName = ui->proName->text();
    QString proPath = ui->proPath->text();

    if(proName.isEmpty() || proName.contains("\\s+"))
    {
        showError("���������Ϸ�");
        return;
    }

    if(proPath.isEmpty() || proName.contains("\\s+"))
    {
        showError("·�������Ϸ�");
        return;
    }

    QString fullName = proPath + "/" + proName;
    QFileInfo info(fullName);
    if(info.exists())
    {
        showError("�����Ѿ�����");
        return;
    }

    ui->error->hide();

    isInfoFilled = true;
    emit completeChanged();
}

//��ʾ����·��ѡ���
void MyWelcomePage::showFilePathSelect()
{
    QString selectedPath = QFileDialog::getExistingDirectory(this,"ѡ��Ŀ¼","/home",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if(!selectedPath.isEmpty())
    {
        ui->proPath->setText(selectedPath);
    }
}

void MyWelcomePage::showError(QString text)
{
    ui->error->show();
    ui->error->setStyleSheet("background-color:red;color:white;");
    ui->error->setText(text);
    isInfoFilled = false;
    emit completeChanged();
}

MyWelcomePage::~MyWelcomePage()
{
    delete ui;
}
