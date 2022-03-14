#include "startmenu.h"
#include <QFileDialog>
#include <QVBoxLayout>

StartMenu::StartMenu(View *v, QWidget *parent) : CentralWidget(v, parent), createfrominput(new QPushButton("create from input")), createfromfile(new QPushButton("get from file")), createfromlast(new QPushButton("get last")){

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(createfrominput);
    l->addWidget(createfromfile);
    l->addWidget(createfromlast);

    QObject::connect(createfrominput, SIGNAL(clicked()), view, SLOT(createFromUserInput()));
    QObject::connect(createfromfile, SIGNAL(clicked()), this, SLOT(createFromFile()));
    QObject::connect(createfromlast, SIGNAL(clicked()), this, SLOT(createFromLast()));
}

int StartMenu::checkChartType(const QString& path){}

void StartMenu::createFromFile(){
    QString path = QFileDialog::getOpenFileName(this, tr("Open Chart"), "", tr("Chart Files (*.json)"));
    //invocazione di checkchartType
    //una volta riconosciuto il tipo invoco sveglio lo slot create...Chart() nella vista il quale
    //si occupa della creazione del model e del ChartWidget adatto
}

void StartMenu::createFromLast(){
    //come sopra
}
