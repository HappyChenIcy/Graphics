/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mygraphicsview.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��:
**Others:
**
**�޸���ʷ:
**20160919:wey:��ӵ���ģʽ
**             ��Ӷ˿ڱ༭ҳ��
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class MyNodePort;
class NodeEditDialog;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static MyGraphicsView * instance();

    MyGraphicsView(QWidget * parent = 0);
    ~MyGraphicsView();

    void showNodePortEdit(MyNodePort * nodePort);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);


private:
    static MyGraphicsView * viewManager;

    NodeEditDialog * nodeEdit;
};

#endif // MYGRAPHICSVIEW_H
