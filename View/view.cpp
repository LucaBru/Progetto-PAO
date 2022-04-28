#include "view.h"
#include "View/CentralWidget/ChartWidget/piechartwidget.h"
#include "Model/piemodel.h"
#include "View/CentralWidget/ChartWidget/linechartwidget.h"
#include "Model/linemodel.h"
#include "View/CentralWidget/ChartWidget/barchartwidget.h"
#include "Model/barmodel.h"
#include <QToolBar>
#include <QFileDialog>

void View::manageOldChart() const{
    if(dynamic_cast<ChartWidget*>(mainW->centralWidget())){
        QMessageBox msgBox;
        msgBox.setText("Chart has been modify");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Save)
            static_cast<ChartWidget*>(mainW->centralWidget())->saveChart();
    }
}

void View::connectToolBarSignalsToSlots(QAction *new_pie_chart, QAction *new_bar_chart, QAction *new_line_chart, QAction *open_from_file) const{
    QObject::connect(open_from_file, SIGNAL(triggered(bool)), this,  SLOT(openFromFile()));
    QObject::connect(new_pie_chart, SIGNAL(triggered(bool)), this, SLOT(newPieChart()));
    QObject::connect(new_line_chart, SIGNAL(triggered(bool)), this, SLOT(newLineChart()));
    QObject::connect(new_bar_chart, SIGNAL(triggered(bool)), this, SLOT(newBarChart()));
}

void View::configToolBarItems() const{
    QAction *open_from_file = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\open.png"), "open from file");
    QAction *new_pie_chart = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\pie.png"), "new pie chart");
    QAction *new_line_chart = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\line.png"), "new line chart");
    QAction *new_bar_chart = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\bar.png"), "new bar chart");
    connectToolBarSignalsToSlots(new_pie_chart, new_bar_chart, new_line_chart, open_from_file);

}

View::View(QObject *parent) : QObject(parent), mainW(new QMainWindow()), tool_bar(new QToolBar()){
    configToolBarItems();
    mainW->addToolBar(tool_bar);
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

QAction* View::addActionToToolBar(const QIcon &icon, const QString &text){
    tool_bar->addAction(icon, text);
}

void View::removeAction(QAction *action){
    tool_bar->removeAction(action);
}


// ----------------- slot --------------------------

void View::openFromFile(){
    manageOldChart();

    QString file_path = QFileDialog::getOpenFileName(mainW, tr("Open Chart"), "..\\Chart-Application\\chart samples", tr("Chart Files (*.json)"));
    QFile file(file_path);

    if(file.open(QFile::ReadOnly)){
        QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
        if(!obj.isEmpty() && !obj.value("type").isUndefined()){
            QString chart_type = obj.value("type").toString();
            ChartWidget *w = nullptr;
            if(chart_type.compare("pie") == 0)
                w = new PieChartWidget(this, new PieModel(this, obj));
            else if(chart_type.compare("line") == 0)
                w = new LineChartWidget(this, new LineModel(this, obj));
            else if(chart_type.compare("bar") == 0)
                w = new BarChartWidget(this, new BarModel(this, obj));
            setCentralWidget(w);
            if(w){
                w->createChartFromModel();
                w->updateFilePath(file_path);
            }
        }
    }
}

void View::newPieChart(){
    manageOldChart();
    setCentralWidget(new PieChartWidget(this, new PieModel(this)));
}

void View::newLineChart(){
    manageOldChart();
    setCentralWidget(new LineChartWidget(this, new LineModel(this)));
}

void View::newBarChart(){
    manageOldChart();
    setCentralWidget(new BarChartWidget(this, new BarModel(this)));
}



