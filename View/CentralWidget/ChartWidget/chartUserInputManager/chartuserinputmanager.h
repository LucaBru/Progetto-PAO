#ifndef CHARTUSERINPUTMANAGER_H
#define CHARTUSERINPUTMANAGER_H
#include<QObject>

class chartUserInputManager : public QObject{
protected:
    virtual void connectSignalsToSlots();
public:
    chartUserInputManager();
private slots:
    void changeTitle();
public slots:
    void saveChart();
    void saveChartAs();
};

#endif // CHARTUSERINPUTMANAGER_H
