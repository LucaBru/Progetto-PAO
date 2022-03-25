#include "view.h"
#include "View/CentralWidget/startmenu.h"
#include "View/CentralWidget/chartfromusermenu.h"
#include "View/CentralWidget/ChartWidget/piechartwidget.h"
#include "Model/piemodel.h"
#include "View/CentralWidget/ChartWidget/linechartwidget.h"
#include "Model/linemodel.h"

View::View(QObject *parent) : QObject(parent), mainW(new QMainWindow()){
   setCentralWidget(new StartMenu(this));
}

View::~View(){
    delete mainW->centralWidget();
    delete mainW;
}

void View::setCentralWidget(QWidget *widget){
    QWidget *old = mainW->centralWidget();
    mainW->setCentralWidget(widget);
    delete old;
}

void View::show() const{
    mainW->showMaximized();
}

QMainWindow* View::getMainWindow() const{
    return mainW;
}

// ----------------- slot --------------------------

void View::createFromUserInput(){
    setCentralWidget(new ChartFromUserMenu(this));
}

void View::createPieChart(){
    setCentralWidget(new PieChartWidget(this, new PieModel(this)));
}

void View::createLineChart(){
    //come create PieChart
}

void View::createBarChart(){
    //come create PieChart
}


