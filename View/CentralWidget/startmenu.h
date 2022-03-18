#ifndef STARTMENU_H
#define STARTMENU_H
#include "View/CentralWidget/centralwidget.h"
#include <QPushButton>

class StartMenu : public CentralWidget{
    Q_OBJECT
private:
    QPushButton *createfrominput;
    QPushButton *createfromfile;
    QPushButton *createfromlast;
public:
    StartMenu(View *v, QWidget *parent =nullptr);
private slots:
    void createFromFile();
    void createFromLast();
};

/*
 * Contratto metodo statico privato checkChartType():
 *  - tenta apertura del file .json avente percorso path:
 *   - se riesce ad aprirlo ritorna:
 *    - 1 se si tratta di un pieChart
 *    - 2 se si tratta di un LineChart
 *    - 3 se si tratta di un barChart
 *    - 0 altrimenti (tipo di file sconosciuto o problemi con apertura file)
 */

#endif // STARTMENU_H
