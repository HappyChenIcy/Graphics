/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: parameterdefinetableview.h
**��  ��: wey       Version: 1.0       Date: 2016.11.11
**��  ��: ����������ͼ(ѭ����)
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef PARAMETERDEFINETABLEVIEW_H
#define PARAMETERDEFINETABLEVIEW_H

#include <QTableView>

#include "../Header.h"

class ParameterDefineDelegate;
class ParameterDefineModel;
class QMenu;
class QAction;
struct LoopProperty;

class ParameterDefineTableView : public QTableView
{
    Q_OBJECT
public:
    ParameterDefineTableView(LoopPart loopPart,int row,int column,QWidget * parent = 0);
    ~ParameterDefineTableView();

    void setLoopItemProp(LoopProperty * prop, QVariant userRoleData);
    ParameterDefineModel * model()const;
    ParameterDefineDelegate * delegate();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void variContentEdited();
    void deleteVari(QString);

private slots:
    void insertRow();
    void deleteRow();

private:
    void initContextMenu();

    QMenu * rightMenu;
    QAction * insertAction;
    QAction * deleteAction;

    LoopPart loopPart;

    ParameterDefineDelegate * paraDelegate;
    ParameterDefineModel * paraModel;
};

#endif // PARAMETERDEFINETABLEVIEW_H
