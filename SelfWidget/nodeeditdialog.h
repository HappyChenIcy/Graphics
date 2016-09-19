/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: nodeeditdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.09.19
**��  ��: ���ڱ༭ĳ���˿�
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef NODEEDITDIALOG_H
#define NODEEDITDIALOG_H

#include <QDialog>

class MyNodePort;

namespace Ui {
class NodeEditDialog;
}

class NodeEditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodeEditDialog(QWidget *parent = 0);
    void initNodeInfo(MyNodePort * node);
    ~NodeEditDialog();
    
private:
    Ui::NodeEditDialog *ui;

    MyNodePort * nodePort;

};

#endif // NODEEDITDIALOG_H
