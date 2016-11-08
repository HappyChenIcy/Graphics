#include "myportinitialdialog.h"
#include "ui_myportinitialdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyPortInitialDialog::MyPortInitialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortInitialDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("��ʼ���˿�����");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    ui->widget_3->setParetWidget(this);

}

MyPortInitialDialog::~MyPortInitialDialog()
{
    delete ui;
}
