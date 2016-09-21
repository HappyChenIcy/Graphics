/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: fileoperate.h
**��  ��: wey       Version: 1.0       Date: 2016.09.09
**��  ��: �ؼ��������
**Others:
**
**�޸���ʷ:
**20160909:wey:��ӶԿؼ��ı���ͽ���
**20160920:wey:��ӶԶ˿ڵı���ͽ���
*************************************************/
#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include <QString>

class QGraphicsItem;

#include "Header.h"

class FileOperate
{
public:
    static FileOperate * instance();

    ReturnType saveFile(QString fileName, const QList<QGraphicsItem *> &items);
    ReturnType openFile(QString fileName, QList<CutInfo *> &items);

private:
    FileOperate();
    static FileOperate * fileOperate;
};

#endif // FILEOPERATE_H
