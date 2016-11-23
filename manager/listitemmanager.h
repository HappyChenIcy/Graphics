/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: listitemmanager.h
**��  ��: wey       Version: 1.0       Date: 2016.11.01
**��  ��: �б�����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef LISTITEMMANAGER_H
#define LISTITEMMANAGER_H

#include <QListWidgetItem>
#include <QObject>

#include "id.h"

class MyListItem: public QListWidgetItem
{
public:
    MyListItem ( QListWidget * parent = 0, int type = Type);
    MyListItem ( const QString & text, QListWidget * parent = 0, int type = Type );
    MyListItem ( const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = Type );
    MyListItem ( const QListWidgetItem & other );
    ~MyListItem();
};

class ListItemManager
{

public:
    ListItemManager();
    static ListItemManager * instance();

    MyListItem * createListItem(const char * name,QListWidget * parent = 0);
    MyListItem * createListItem(const char * name,const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type);
    MyListItem * createListItem(const char * name,const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type);
    MyListItem * createListItem(const char * name,const QListWidgetItem & other);

    bool contains(const char * name);

    ~ListItemManager();

private:
    static ListItemManager * manager;

    //qmap��key���Ϊ�Զ��������ͣ���ôҪ����<�����
    QMap<Id,MyListItem *> listItemMap;

};

#endif // LISTITEMMANAGER_H
