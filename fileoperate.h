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
**20160921:wey:��ӶԶ˿ڼ�ͷ�ı���ͽ���
**20160929:wey:��ӶԽ����ļ���������ȵ��ж������ҳ�������ȣ�������Ϊ������ؼ������ֵ
**20161017:wey:�޸��ؼ�����-��-�����Σ������ϵ�������Ϣ�ظ����档
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
