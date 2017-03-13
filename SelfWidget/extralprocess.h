/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: extralprocess.h
**��  ��: wey       Version: 1.0       Date: 2017.03.13
**��  ��:
**Others:�ⲿ�����������ڣ�֧��·��ѡ�񣬲�������
**
**�޸���ʷ:
**20170313:wey:��Ӽ�¼����������Ϣ
*************************************************/
#ifndef EXTRALPROCESS_H
#define EXTRALPROCESS_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class ExtralProcess;
}

class QProcess;

class ExtralProcess : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExtralProcess(QWidget *parent = 0);

    static ExtralProcess * instance();
    ~ExtralProcess();

private slots:
    void openLocalFile();
    void startProcess();
    void stopProcess();
    void respStateChanged(QProcess::ProcessState state);
    
private:
    void addText(QString info);

    Ui::ExtralProcess *ui;

    static ExtralProcess * extralProcess;

    QProcess * process;
};

#endif // EXTRALPROCESS_H
