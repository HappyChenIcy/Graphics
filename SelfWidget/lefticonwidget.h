/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: lefticonwidget.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��: ��������ؼ�����
**Others:
**       1.֧�ֿ���ק
**       2.֧�ַ���鿴
**�޸���ʷ:
**
*************************************************/
#ifndef LEFTICONWIDGET_H
#define LEFTICONWIDGET_H

#include <QWidget>

class QToolBox;
class MyListWidget;

class LeftIconWidget : public QWidget
{
    Q_OBJECT
public:
    LeftIconWidget(QWidget * parent = 0);
    ~LeftIconWidget();

private:
    void initWidget();
    void initListItems();

    QToolBox * toolBox;
    MyListWidget * polygonWidget;         //���Ρ�Բ��
    MyListWidget * nodeWidget;            //�˿ڵ�С��������Ҫ���뵽polygonWidget֮��

};

#endif // LEFTICONWIDGET_H
