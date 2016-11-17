#include "myportoutputdialog.h"
#include "ui_myportoutputdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyPortOutputDialog::MyPortOutputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortOutputDialog)
{
    ui->setupUi(this);

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    chooseBar = new MyChooseBar(ui->widget_3);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout);

    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(updateProp()));
}

void MyPortOutputDialog::setInOutState(bool isInstate)
{
    ui->portTypeBox->clear();
    QStringList portType;
    if(isInstate)
    {
        setWindowTitle("����˿�����");
        portType<<"�¼���"<<"�¼���1";
        ui->portTypeBox->addItems(portType);
    }
    else
    {
        setWindowTitle("����˿�����");
        portType<<"������"<<"������2";
        ui->portTypeBox->addItems(portType);
    }
}

void MyPortOutputDialog::setProp(StateInOutProperty &prop)
{
    ui->portName->setText(prop.portName);
}

//��������
void MyPortOutputDialog::updateProp()
{
    sprop.portName = ui->portName->text();
    sprop.portType = ui->portTypeBox->currentText();
}

StateInOutProperty & MyPortOutputDialog::getProp()
{
    return this->sprop;
}

MyPortOutputDialog::~MyPortOutputDialog()
{
    delete ui;
}
