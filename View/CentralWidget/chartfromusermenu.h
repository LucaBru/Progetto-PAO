#ifndef CHARTFROMUSERMENU_H
#define CHARTFROMUSERMENU_H
#include "View/CentralWidget/centralwidget.h"
#include <QPushButton>

class ChartFromUserMenu : public CentralWidget{
private:
    QPushButton *createpiechart;
    QPushButton *createlinechart;
    QPushButton *createbarchart;
public:
    ChartFromUserMenu(View *v, QWidget *parent =nullptr);

};

#endif // CHARTFROMUSERMENU_H
