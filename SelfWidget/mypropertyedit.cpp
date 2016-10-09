#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

#include "../global.h"

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle("����༭");

    int width = 550;
    int height = 600;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - height)/2,width,height);

    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(confirmPropety()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(cancelProperty()));
}

//ȷ��
void MyPropertyEdit::confirmPropety()
{

}

//ȡ��
void MyPropertyEdit::cancelProperty()
{

}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
