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
#include <QMap>

#include "../Header.h"

namespace Ui {
class HideSplit;
}

class HideSplit : public QWidget
{
    Q_OBJECT
    
public:
    explicit HideSplit(SpiltDirect direct = SPLIT_LEFT,QWidget *parent = 0);

    void setFixedWidth(int w);

    void addWidget(QWidget * widget);
    QWidget * getContainer();

    ~HideSplit();

public slots:
    void setContainerVisible();
    
private:
    void init();

    Ui::HideSplit *ui;
    SpiltDirect splitDirect;        //���ع�������������λ�ù�ϵ
    int fixWidth;
};

class SplitManager : public QObject
{
    Q_OBJECT
public:
    SplitManager();
    static SplitManager * instance();
    HideSplit * addSplit(QString Id,SpiltDirect direct,QWidget * widget);
    HideSplit * split(QString Id);

private:
    static SplitManager * manger;

    QMap<QString,HideSplit*> maps;
};

#endif // HIDESPLIT_H
