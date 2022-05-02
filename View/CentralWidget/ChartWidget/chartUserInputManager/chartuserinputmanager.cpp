#include "chartuserinputmanager.h"

void chartUserInputManager::connectSignalsToSlots(){
    QObject::connect(chart_title, SIGNAL(editingFinished()), this, SLOT(changeTitle()));
    QObject::connect(save, SIGNAL(triggered(bool)), this, SLOT(saveChart()));
    QObject::connect(save_as, SIGNAL(triggered(bool)), this, SLOT(saveChartAs()));
}

chartUserInputManager::chartUserInputManager()
{

}
