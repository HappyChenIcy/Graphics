#include "nodeeditdialog.h"
#include "ui_nodeeditdialog.h"

#include "../item/mynodeport.h"

NodeEditDialog::NodeEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeEditDialog)
{
    ui->setupUi(this);

    setWindowTitle("�˿����Ա༭");

}

//��ʼ���ڵ���Ϣ��Ϊ�༭��׼��
void NodeEditDialog::initNodeInfo(MyNodePort *node)
{
    this->nodePort = node;
}

NodeEditDialog::~NodeEditDialog()
{
    delete ui;
}
