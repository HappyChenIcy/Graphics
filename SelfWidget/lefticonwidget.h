/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: lefticonwidget.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��: ��������ؼ�����
**Others:
**       1.֧�ֿ���ק
**       2.֧�ַ���鿴
**�޸���ʷ:
**20160927:wey:����ƽ���ı���
**20160929:wey:����ѭ������/ѭ������/����/ע��
**20161027:wey:�������Ǻ�D�˿�
*************************************************/
#ifndef LEFTICONWIDGET_H
#define LEFTICONWIDGET_H

#include <QWidget>
#include "../Header.h"

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
#ifdef ADD_STATE_MODEL
    MyListWidget * stateWidget;           //״̬��
    MyListWidget * maskWidget;            //����
#endif
    MyListWidget * nodeWidget;            //�˿ڵ�С��������Ҫ���뵽polygonWidget֮��

};

#endif // LEFTICONWIDGET_H
