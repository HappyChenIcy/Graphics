/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: menumanager.h
**��  ��: wey       Version: 1.0       Date: 2016.10.28
**��  ��:
**Others: ����˵�
**
**�޸���ʷ:
**
*************************************************/
#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QObject>
#include <QMenu>

#include "Constants.h"
#include "Header.h"
#include "id.h"

using namespace Graphics;

class MyMenu : public QMenu
{
    Q_OBJECT
public:
    MyMenu( QWidget * parent = 0 );
    MyMenu( const QString title,QWidget * parent = 0);
};

class MenuManager
{
public:
    MenuManager();
    static MenuManager * instance();

    MyMenu * createMenu(const char *name, QWidget *parent = 0);
    MyMenu * createMenu(const char *name, const QString title, QWidget * parent = 0);
    MyMenu * menu(const char *name);

    bool contains(const char * name);

private:
    MyMenu * value(const char *name);
    static MenuManager * manager;

    //qmap��key���Ϊ�Զ��������ͣ���ôҪ����<�����
    QMap<Id,MyMenu *> menuMap;
};

#endif // MENUMANAGER_H
