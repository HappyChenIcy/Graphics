/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mygraphicsview.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��:
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget * parent = 0);
    ~MyGraphicsView();
};

#endif // MYGRAPHICSVIEW_H
