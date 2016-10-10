/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: serviceinputtableview.h
**��  ��: wey       Version: 1.0       Date: 2016.10.09
**��  ��:
**Others: �������Ա༭��ͼ
**
**�޸���ʷ:
**20161010:wey:��Ӳ���/ɾ����
*************************************************/
#ifndef SERVICEINPUTTABLEVIEW_H
#define SERVICEINPUTTABLEVIEW_H

#include <QTableView>

class ServiceInputModel;
class ServiceInputDelegate;
class QMenu;
class QAction;

class ServiceInputTableView : public QTableView
{
    Q_OBJECT
public:
    ServiceInputTableView(int row,int column,QWidget * parent = 0);

    ServiceInputModel * model() const;

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void insertRow();
    void deleteRow();

private:
    void initContextMenu();

    ServiceInputModel * serviceModel;
    ServiceInputDelegate * serviceDelegate;

    QMenu * rightMenu;
    QAction * insertAction;
    QAction * deleteAction;
};

#endif // SERVICEINPUTTABLEVIEW_H
