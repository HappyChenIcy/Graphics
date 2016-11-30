/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: simulatecontrolpanel.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others:���ݿ������
**
**�޸���ʷ:
**20161017:wey:����ģ����棬����ģ��ǰ�Կؼ�״̬�Ļָ�
**20161020:wey:������֤��ʾ���Լ��������й���
**20161021:wey:���Ӵ����ݶ���ѡ���Ӧ�Ĵ���Ԫ
**20161025:wey:���Ӵӹ����б�ֱ��˫����ʾ����Ԫ���Ա༭��Ϣ
**20161104:wey:���ӽ�ֹ������ģ���״̬�˳�����ģ״̬
**20161107:wey:�޸���ȡ���Ĵ���Ԫ�����жϽڵ������ӽڵ�Ϊnull�����·��ʳ�������
**20161110:wey:���ӷ�������
**             �������ݹ���������ǰ�ߵ������Ϊ�Լ�������
**20161114:wey:���Ӷ�ѭ����Ƕ��ѭ����֧��(Ŀǰ��֧��һ������ѭ���ж�)
**20161115:wey:�޸�ѭ������--�Ĵ���
**             ����Զ��͵�������
**20161117:wey:����ѭ��������������
**             �������ݸ�����ʾ(����ָʾ��ǰ���ݵĽ���)
**             ���Ӷ�ѭ�����̵ļ�¼��֧�ֵ���Ҳ��¼�鿴��ǰѭ����ֵ
**20161123:wey:�����жϿ������Ĵʷ��������߼�����
**             �������񷵻ؽ�����������װ��map����
**20161130:wey:���Ӷ�����������֧��
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>
#include <QListWidgetItem>

#include "../global.h"

class QLabel;
class MyItem;
struct ProcessUnit;
struct ServiceProperty;

namespace Ui {
class SimulateControlPanel;
}

//ѭ����¼
struct LoopRecord
{
    LoopRecord()
    {
        item = NULL;
        record = 0;
    }
    MyItem * item;
    int record;         //ѭ��ִ�еĴ���
};

class MyListWidgetItem : public QListWidgetItem
{
public:
    MyListWidgetItem(QListWidget * parent = 0, int type = Type);
    MyListWidgetItem(const QString & text, QListWidget * parent = 0, int type = Type);

    void bindProscessUnit(ProcessUnit *unit);
    ProcessUnit * getUnit(){return this->punit;}

private:
    ProcessUnit * punit;
};

class SimulateControlPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit SimulateControlPanel(QWidget *parent = 0);
    ~SimulateControlPanel();

    void resetSimluateFlag();

signals:
    void resetSimluate();
    void sendSingleSimulate(ProcessUnit * unit);

private slots:
    void respStartSimulate();
    MyListWidgetItem * showSimulateOperate(ProcessUnit * unit);
    void respItemActivated(QListWidgetItem * current);
    void respItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
    void respItemDoubleClicked(QListWidgetItem * current);
    void procLastUnitResult(bool hasFault, QMap<QString, QString> result);
    void stopCurrSimulate();
    void chooseRunMethod(bool flag);
    void stepByStep();
    
private:
    void setFlagState(QLabel * label,bool isSuccess);
    void setSimulateState(bool isSim);
    void submitUrl(MyItem *item, ServiceProperty * prop);
    void startProcUnit();
    bool countLoopValue(MyItem *item, SignalVariList & loopList);
    void clearLastSimluteRecord();
    QString getQuoteOutValue(MyItem * item,QString value);
    bool countJudgeValue(MyItem *item, QString express);
    QString switchQuoteParameter(MyItem *item, QString & express);
    QString findQuoteResult(MyItem * item, QString quoteServiceName, QString quoteParaName);
    void showCurrProcessResultPanel(bool isPanelEditable);

    Ui::SimulateControlPanel *ui;

    QList<ProcessUnit *> procUnits;

    bool isSimulateState;                   //�Ƿ�Ϊģ��״̬
    bool isAutoRun;                         //�Ƿ�Ϊ�Զ�����

    ProcessUnit * currProcUnit;             //��ǰ����Ԫ
    ProcessUnit * beforeUnit;               //֮ǰ����Ԫ(���ݽ�����ΪNULL)

    QList<LoopRecord *>  loopRecords;       //��¼ÿ��ѭ����״̬��Ϣ
};

#endif // SIMULATECONTROLPANEL_H
