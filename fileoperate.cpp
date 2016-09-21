#include "fileoperate.h"

#include <QGraphicsItem>

#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "./item/mynodeport.h"
#include "global.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

//#include "typeinfo.h"
#include "typeinfo"

FileOperate::FileOperate()
{

}

FileOperate * FileOperate::instance()
{
    if(!fileOperate)
    {
        fileOperate = new FileOperate();
    }
    return fileOperate;
}

FileOperate * FileOperate::fileOperate =  NULL;

ReturnType FileOperate::saveFile(QString fileName,const QList<QGraphicsItem *> &items)
{
    if(!fileName.contains(SaveFileSuffix))
    {
        fileName += SaveFileSuffix;
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return FILE_CANT_WRITE;
    }

    QDataStream stream(&file);

    //д���ļ���ʶͷ
    stream<<SaveFileHeadFlag;

    foreach(QGraphicsItem * item,items)
    {
        QString itemName = typeid(*item).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * myItem = dynamic_cast<MyItem *>(item);
            stream<<myItem;
        }
        else if(itemName == typeid(MyArrow).name())
        {
            MyArrow * myItem = dynamic_cast<MyArrow *>(item);
            stream<<myItem;

//            qDebug()<<myItem->getProperty().startItemID<<"_arrow_"<<myItem->getProperty().endItemID<<"_"<<__FUNCTION__;
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * myItem = dynamic_cast<MyTextItem *>(item);
            stream<<myItem;
        }
        else if(itemName == typeid(MyNodePort).name())
        {
            MyNodePort * myItem = dynamic_cast<MyNodePort *>(item);
            stream<<myItem;
//            qDebug()<< myItem->getNodeProperty().startItemID<<"_______"<<__FUNCTION__;
        }
    }
    file.close();
    return RETURN_OK;
}

/**
 *���ؽ����ļ�,��˿�NodePort������NodePortProperty�ǵ������棬��Ҫ�����������ж϶�ȡ
*��ȡ��ɺ�ֱ�ӽ��˿ڽ���װ����ÿ��Item��,ʹ�õ��ò�����֪�������ϸ�ڲ���
*/
ReturnType FileOperate::openFile(QString fileName,QList<CutInfo *> &items)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return FILE_CANT_READ;
    }

    QDataStream stream(&file);

    QString fileFlag;
    stream>>fileFlag;
    if(fileFlag != SaveFileHeadFlag)
    {
        file.close();
        return FILE_ILLEGAL;
    }

    QList<NodePortProperty > localNodePorts;

    //�ļ������ȡ
    while(!stream.atEnd())
    {
        int type;
        stream>>type;

        GraphicsType gtype = (GraphicsType)type;

        if(gtype == GRA_NODE_PORT)
        {
            NodePortProperty nodeProperty;
            stream>>nodeProperty;
            localNodePorts.push_back(nodeProperty);
        }
        else
        {
            CutInfo * info = new CutInfo;
            stream>>info->itemProperty;
            info->graphicsType = (GraphicsType)type;
            items.push_back(info);
        }
    }

    //�˿�װ��
    if(localNodePorts.size() > 0)
    {
        for(int i = 0;i<localNodePorts.size();i++)
        {
            for(int j = 0; j < items.size() ;j++)
            {
                if(items.at(j)->itemProperty.startItemID == localNodePorts.at(i).parentItemID)
                {
                    NodePortProperty prop = localNodePorts.at(i);
                    items.at(j)->nodeProperties.push_back(prop);
                }
            }
        }
    }

    return RETURN_OK;
}
