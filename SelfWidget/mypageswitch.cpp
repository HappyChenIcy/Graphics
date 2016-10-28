#include "mypageswitch.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>

#include "../manager/mypageitem.h"
#include "../manager/actionmanager.h"
#include "../manager/menumanager.h"
#include "../util.h"
#include "../item/myscene.h"
#include "../item/mygraphicsview.h"
#include "../global.h"
#include "../Constants.h"
#include "../mainwindow.h"
#include "Header.h"

using namespace Graphics;

MyPageSwitch::MyPageSwitch(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(25);
    isFirstView = false;

    pagePosition = 0;
    selectedPage = NULL;
    initWidget();
}

void MyPageSwitch::initWidget()
{
    addPageButt = new QPushButton();
    addPageButt->setFixedSize(20,20);
    addPageButt->setToolTip("���빤��ҳ(Ctrl+N)");
    addPageButt->setText("+");
    connect(addPageButt,SIGNAL(clicked()),this,SLOT(addPage()));

    layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(addPageButt);
    layout->addStretch(0);
    setLayout(layout);
}

void MyPageSwitch::addSwitchContextMenu()
{
    MyMenu * rightMenu = MenuManager::instance()->createMenu(Constants::MENU_MYPAGE_SWITCH);
    rightMenu->addAction(ActionManager::instance()->action(Constants::CLOSE_WORKSPACE));
    rightMenu->addAction(ActionManager::instance()->action(Constants::SAVE_WORKSPACE));
    rightMenu->addAction(ActionManager::instance()->action(Constants::CLOSE_LEFT_WORKSPACE));
    rightMenu->addAction(ActionManager::instance()->action(Constants::CLOSE_RIGHT_WORKSPACE));
}

//���ҳ��
void MyPageSwitch::addPage()
{
    QString id = Util::getUUID();

    MyPageItem  * item = PageManager::instance()->addPageItem();
    connect(item,SIGNAL(switchPage(QString)),this,SLOT(switchToPage(QString)));
    connect(item,SIGNAL(deletePage(QString)),this,SLOT(deleteThisPage(QString)));
    item->setId(id);

    PageMapping * page = new PageMapping;
    page->id = id;
    page->pageItem = item;
    page->scene = MyGraphicsView::instance()->addScene(id);
    page->scaleView = 100;
    page->hScrollValue = SceneWidth /2;
    page->vScrollValue = SceneHeight /2;
    page->pageName = QString("������%1").arg(PageManager::instance()->getPageCount());
    selectedPage = page;

    pages.append(page);
    layout->insertWidget(++pagePosition,item);
    item->setText(page->pageName);

    if(!isFirstView)
    {
        isFirstView = true;
        siwtchPage(id,isFirstView);
    }
    else
    {
        switchToPage(id);
    }
}

//�л�ҳ��.�л�ǰ��Ҫ�ȱ��浱ǰscene�еĳߴ���Ϣ
void MyPageSwitch::switchToPage(QString pageId)
{
    siwtchPage(pageId);
}

//�л�ҳ��
void MyPageSwitch::siwtchPage(QString pageId, bool firstView)
{
    if(selectedPage && !firstView)
    {
        selectedPage->hScrollValue = MyGraphicsView::instance()->getHorizonalValue();
        selectedPage->vScrollValue = MyGraphicsView::instance()->getVertiaclValue();
        selectedPage->scaleView = MyGraphicsView::instance()->getScaleValue();
    }

    foreach (PageMapping * mapping, pages)
    {
        if(mapping->id == pageId)
        {
            selectedPage = mapping;
            mapping->pageItem->setSelected(true);
            MyGraphicsView::instance()->transformView(mapping->hScrollValue,mapping->vScrollValue);
            MyGraphicsView::instance()->setScaleValue(mapping->scaleView);
            MyGraphicsView::instance()->showScene(mapping->scene);
            emit switchPage();
        }
        else
        {
            mapping->pageItem->setSelected(false);
        }
    }
}

//ɾ����ǰҳ�棬�ú���һ��ҳ��
void MyPageSwitch::deleteThisPage(QString pageId)
{
    int pageIndex = getPageIndex(pageId);

    QString tipInfo;

    if(pageIndex >= 0)
    {
        tipInfo = QString("�Ƿ�ɾ�� [%1]?").arg(pages.at(pageIndex)->pageName);
    }
    else
    {
        tipInfo = "�Ƿ�ɾ���˹�����?";
    }

    int result = Util::getWarnChoice(tipInfo);
    if(result == QMessageBox::Yes)
    {
        int index = -1;
        for( int i = 0; i < pages.size() ; i++)
        {
            if(pages.at(i)->id == pageId )
            {
                index = i;
                break;
            }
        }
        if( index >=0  && index < pages.size()-1)
        {
            switchToPage(pages.at(index + 1)->id);
        }
        else if(index == pages.size()-1 && index >0)
        {
            switchToPage(pages.at(index - 1)->id);
        }
        else if(index <0)
        {
            return;
        }

        pages.at(index)->scene->clear();

        delete pages.at(index)->scene;
        delete pages.at(index)->pageItem;
        pages.removeAt(index);

        --pagePosition;
        if(pages.size() == 0)
        {
            MyGraphicsView::instance()->deleteScene();
        }
        emit deletePage();
    }
}

//�л�ǰһ��ҳ��
void MyPageSwitch::switchFrontBack(bool isFront)
{
    MY_ASSERT_GT_ZERO(pages.size())

    int index = -1;
    for(int i = 0;i<pages.size(); i++)
    {
        if(selectedPage->id == pages.at(i)->id)
        {
            index = i;
            break;
        }
    }

    if(isFront && index >0)
    {
        switchToPage(pages.at(index -1 )->id);
    }
    else if(!isFront && index < pages.size() - 1)
    {
        switchToPage(pages.at(index +1 )->id);
    }
}

//�رյ�ǰҳ
void MyPageSwitch::closePage()
{
    if(pages.size() > 0)
    {
        deleteThisPage(selectedPage->id);
    }
}

//�ر�������й�����
void MyPageSwitch::closeLeftPage()
{
    int index = getPageIndex(selectedPage->id);
    if(index > 0)
    {
        for(int i = index - 1;i>=0;i--)
        {
            deleteThisPage(pages.at(i)->id);
        }
    }
}

//�ر��Ҳ����й�����
void MyPageSwitch::closeRightPage()
{
    int index = getPageIndex(selectedPage->id);

    if(index >= 0)
    {
        for(int i = pages.size() -1; i > index ;i--)
        {
            deleteThisPage(pages.at(i)->id);
        }
    }
}

//��ȡ�����ڼ����е�λ��
int MyPageSwitch::getPageIndex(QString id)
{
    for(int i = 0;i<pages.size();i++)
    {
        if(pages.at(i)->id == id)
        {
            return i;
        }
    }
    return -1;
}

MyPageSwitch * MyPageSwitch::pageSwitch = NULL;

MyPageSwitch * MyPageSwitch::instance()
{
    if(!pageSwitch)
    {
        pageSwitch = new MyPageSwitch;
    }
    return pageSwitch;
}
