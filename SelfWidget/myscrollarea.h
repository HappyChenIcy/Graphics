/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypageitemscrollarea.h
**��  ��: wey       Version: 1.0       Date: 2016.11.06
**��  ��: �Զ���Ļ�����������
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QWidget>

class QHBoxLayout;

namespace Ui {
class MyScrollArea;
}

class MyScrollArea : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyScrollArea(QWidget *parent = 0);
    ~MyScrollArea();

    void resizeWidgetPos();

    void insertWidget(int index,QWidget * item);
    
protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::MyScrollArea *ui;
    QWidget * widget;

    QHBoxLayout * layout;
    QWidget * itemContainer;
};

#endif // MYSCROLLAREA_H
