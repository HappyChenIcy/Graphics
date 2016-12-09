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
**20161020:wey:��Ӵ��ļ��뵱ǰ�汾�ıȶ�
**20161027:wey:���D�����Ƕ˿ڶ�д
**20161028:wey:��ӶԶ˿��������ݶ�д
**20161115:wey:��ӶԷ�������ԡ�ѭ�������Զ�д
**20161121:wey:��Ӷ��ļ����汾�ŵ�д�룬����֪��ģ�͵İ汾
**20161209:wey:��Ӷ����ߵı���Ͷ�ȡ��ͬʱ�ڼ�����Ϣ������۵㼯��
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

    ReturnType saveFile(QString &fileName, const QList<QGraphicsItem *> &items);
    ReturnType openFile(QString fileName, QList<CutInfo *> &items);

private:
    FileOperate();
    static FileOperate * fileOperate;
};

#endif // FILEOPERATE_H
