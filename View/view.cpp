#include "view.h"
#include "View/CentralWidget/startmenu.h"
#include "View/CentralWidget/chartfromusermenu.h"
#include "View/CentralWidget/ChartWidget/piechartwidget.h"
#include "Model/piemodel.h"

View::View(QObject *parent) : QObject(parent), mainW(new QMainWindow()){
   //setCentralWidget(new StartMenu(this));
   PieModel *model = new PieModel(this);
   setCentralWidget(new PieChartWidget(this, model));
}

View::~View(){
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
    //creo un PieModel,
    //setCentralWidget(PieWidget) <- passandogli il model appena creato
}

void View::createLineChart(){
    //come create PieChart
}

void View::createBarChart(){
    //come create PieChart
}


