/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mytextinput.h
**��  ��: wey       Version: 1.0       Date: 2016.09.20
**��  ��:
**Others: �ı��༭��
**
**�޸���ʷ:
**20160927:wey:����lineEdit��plainTextEdit
*************************************************/
#ifndef MYTEXTINPUT_H
#define MYTEXTINPUT_H

#include <QDialog>

namespace Ui {
class MyTextInput;
}

class MyTextInput : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyTextInput(QWidget *parent = 0);
    ~MyTextInput();

    void setTex(QString text);
    QString getText();
    
private:
    Ui::MyTextInput *ui;
};

#endif // MYTEXTINPUT_H
