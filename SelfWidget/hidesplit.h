/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: hidesplit.h
**��  ��: wey       Version: 1.0       Date: 2016.09.25
**��  ��:
**Others:�Ҳ����������ʾ���ƴ���
**
**�޸���ʷ:
**
*************************************************/
#ifndef HIDESPLIT_H
#define HIDESPLIT_H

#include <QWidget>

#include "../Header.h"

namespace Ui {
class HideSplit;
}

class HideSplit : public QWidget
{
    Q_OBJECT
    
public:
    explicit HideSplit(SpiltDirect direct = SPLIT_LEFT,QWidget *parent = 0);

    static HideSplit *addWidget(SpiltDirect direct,QWidget * widget);

    void setFixedWidth(int w);

    void addWidget(QWidget * widget);
    QWidget * getContainer();

    ~HideSplit();

private slots:
    void setContainerVisible();
    
private:
    void init();

    Ui::HideSplit *ui;
    SpiltDirect splitDirect;        //���ع�������������λ�ù�ϵ
    int fixWidth;

};

#endif // HIDESPLIT_H
