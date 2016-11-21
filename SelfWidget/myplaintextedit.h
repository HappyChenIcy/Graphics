/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myplaintextedit.h
**��  ��: wey       Version: 1.0       Date: 2016.11.18
**��  ��: �Զ����ı��༭��
**Others:ʵ�ֶ����ò������е�ѡ���Լ�֧��backspace��delete�ؼ������ò����Ķ�̬�жϡ�
**
**�޸���ʷ:
**20161121:wey:���Ӷ�backspace��delete��֧��
*************************************************/
#ifndef MYPLAINTEXTEDIT_H
#define MYPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class MyPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    MyPlainTextEdit(QWidget * parent = 0);

protected:
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    //��Ѱ����
    enum SearchDirect
    {
        SEARCH_BEFORE,          //��ǰ��Ѱ
        SEARCH_AFTER            //�����Ѱ
    };


    int findBeforePos(QString source,int curPos,QChar character);
    int findAfterPos(QString source,int curPos,QChar character);
    bool searchQuoteParameter(SearchDirect direct = SEARCH_BEFORE);
};

#endif // MYPLAINTEXTEDIT_H
