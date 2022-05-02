#ifndef PIECHARTUSERINPUTMANAGER_H
#define PIECHARTUSERINPUTMANAGER_H
#include <QObject>

class pieChartUserInputManager : public QObject{
protected:
    virtual void connectSignalsToSlot();
public:
    pieChartUserInputManager();
};

#endif // PIECHARTUSERINPUTMANAGER_H
