#include "startmenu.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QJsonDocument>
#include "View/CentralWidget/ChartWidget/piechartwidget.h"
#include "View/CentralWidget/ChartWidget/linechartwidget.h"
#include "Model/linemodel.h"


StartMenu::StartMenu(View *v, QWidget *parent) : CentralWidget(v, parent), createfrominput(new QPushButton("create from input")), createfromfile(new QPushButton("get from file")), createfromlast(new QPushButton("get last")){

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(createfrominput);
    l->addWidget(createfromfile);
    l->addWidget(createfromlast);

    QObject::connect(createfrominput, SIGNAL(clicked()), view, SLOT(createFromUserInput()));
    QObject::connect(createfromfile, SIGNAL(clicked()), this, SLOT(createFromFile()));
    QObject::connect(createfromlast, SIGNAL(clicked()), this, SLOT(createFromLast()));
}

void StartMenu::createFromFile(){
    QString path = QFileDialog::getOpenFileName(this, tr("Open Chart"), "", tr("Chart Files (*.json)"));
    QFile file(path);

    if(file.open(QFile::ReadOnly)){
        QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
        if(!obj.isEmpty() && !obj.value("type").isUndefined()){
            QString chart_type = obj.value("type").toString();
            if(chart_type.compare("pie") == 0){
                PieChartWidget *w = new PieChartWidget(view, new PieModel(view, obj));
                view->setCentralWidget(w);
                w->createChartFromModel();
                w->setCurrentChartPath(path);
            }
            else if(chart_type.compare("line") == 0){
                LineChartWidget *w = new LineChartWidget(view, new LineModel(view, obj));
                w->createChartFromModel();
                view->setCentralWidget(w);
                w->setCurrentChartPath(path);
            }
            else{}
        }
    }
    //invocazione di checkchartType
    //una volta riconosciuto il tipo invoco sveglio lo slot create...Chart() nella vista il quale
    //si occupa della creazione del model e del ChartWidget adatto
}

void StartMenu::createFromLast(){
    //come sopra
}
