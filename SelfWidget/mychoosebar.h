/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mychoosebar.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: yes/noѡ���
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYCHOOSEBAR_H
#define MYCHOOSEBAR_H

#include <QWidget>

namespace Ui {
class MyChooseBar;
}

class MyChooseBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyChooseBar(QWidget *parent = 0);
    ~MyChooseBar();

    void setParetWidget(QWidget * parent);

signals:
    void confirmPressed();
    void cancelPressed();

private slots:
    void respCancel();
    void respConfirm();
    
private:
    Ui::MyChooseBar *ui;

    QWidget * pWidget;
};

#endif // MYCHOOSEBAR_H
