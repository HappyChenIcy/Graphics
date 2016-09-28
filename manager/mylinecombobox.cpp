#include "mylinecombobox.h"

#include <QListView>
#include <QDebug>

MyLineComboBox::MyLineComboBox()
{

    setView(new QListView());
    setStyleSheet("QComboBox{ height:21px;\
                  }\
                  QComboBox QAbstractItemView::item{\
                  width:150px;height:30px;selection-background-color: lightgray;\
                 }");
    setSizeAdjustPolicy(QComboBox::AdjustToContents);

    isAutoChanged = true;

    connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(respIndexChanged(int)));
}

//��������
void MyLineComboBox::updateIndex(int index)
{
    isAutoChanged = false;
    setCurrentIndex(index);
    isAutoChanged = true;
}

//��Ӧ�л�
void MyLineComboBox::respIndexChanged(int index)
{
    if(isAutoChanged)
    {
        emit indexChanged(index);
    }
}

ComboBoxManager::ComboBoxManager()
{

}

ComboBoxManager * ComboBoxManager::manager = NULL;

ComboBoxManager * ComboBoxManager::instance()
{
    if(!manager)
    {
        manager = new ComboBoxManager();
    }
    return manager;
}

//����ӿؼ�
MyLineComboBox * ComboBoxManager::addItem(const char *name)
{
    if(contains(name))
    {
        return maps.value(Id(name));
    }

    MyLineComboBox * box = new MyLineComboBox;
    box->setObjectName(QString(name));
    maps.insert(Id(name),box);
    return box;
}

//�������Ʒ���item
MyLineComboBox * ComboBoxManager::item(const char *name)
{
    if(contains(name))
    {
        return maps.value(Id(name));
    }
    return NULL;
}

bool ComboBoxManager::contains(const char *name)
{
    QMapIterator<Id,MyLineComboBox *> iterator(maps);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.key().getIdName()==QString(name))
        {
            return true;
        }
    }
    return false;
}

