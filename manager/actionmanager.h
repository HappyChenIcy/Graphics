#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include <QAction>

#include "Constants.h"
#include "Header.h"
#include "id.h"

class MyAction : public QAction
{
    Q_OBJECT
public:
    MyAction(QObject * parent);
    MyAction(const QString & text,QObject *parent);
    MyAction(const QIcon & icon,const QString & text,QObject *parent);
    void setType(GraphicsType type){this->type = type;}
    GraphicsType getType(){return this->type;}
    ~MyAction();

private:
    GraphicsType type;

};

#include <QMap>
class MyItem;

class ActionManager
{

public:
    ActionManager();
    static ActionManager * instance();
    MyAction * crateAction(const char *name, QObject * parent = 0);
    MyAction * crateAction(const char *name, const QString & text, QObject *parent = 0 );
    MyAction * crateAction(const char *name,const QIcon & icon,const QString & text,QObject *parent = 0);
    MyAction * action(const char *name);

    bool registerAction(MyAction *action,QWidget * parent,const char *slot,bool isToggled = false);
    bool contains(const char * name);

    ~ActionManager();

private:
    MyAction * value(const char *name);

    static ActionManager * manager;

    //qmap��key���Ϊ�Զ��������ͣ���ôҪ����<�����
    QMap<Id,MyAction *> actionMap;

};

#endif // ACTIONMANAGER_H
