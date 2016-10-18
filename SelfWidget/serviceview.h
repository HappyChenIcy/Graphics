/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: serviceview.h
**��  ��: wey       Version: 1.0       Date: 2016.10.18
**��  ��:
**Others:�鿴�������Ϣ
**
**�޸���ʷ:
**
*************************************************/
#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QDialog>

#include "../Header.h"

class PageSwitchBar;
class QTableWidget;

namespace Ui {
class ServiceView;
}

class ServiceView : public QDialog
{
    Q_OBJECT
    
public:
    static ServiceView * instance();
    ~ServiceView();

private slots:
    void viewDatabaseContent();
    void refreshDatabaseContent();
    void beginSearch();                     //��ʼ��ѯ

private:
    void initComponent();
    void resizeTable(QTableWidget * table);

    explicit ServiceView(QWidget *parent = 0);
    static ServiceView * serviceView;

    Ui::ServiceView *ui;

    PageSwitchBar * pageBar;

    RowList rowIndexs;

};

#endif // SERVICEVIEW_H
