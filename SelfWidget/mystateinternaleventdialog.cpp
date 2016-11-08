#include "mystateinternaleventdialog.h"
#include "ui_mystateinternaleventdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyStateInternalEventDialog::MyStateInternalEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateInternalEventDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("״̬�ڲ��¼�(��������)����");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    ui->widget_3->setParetWidget(this);
}

MyStateInternalEventDialog::~MyStateInternalEventDialog()
{
    delete ui;
}
