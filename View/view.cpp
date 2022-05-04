#include "view.h"
#include "View/CentralWidget/ChartWidget/piechartwidget.h"
#include "Model/piemodel.h"
#include "View/CentralWidget/ChartWidget/XYChartWidget/linechartwidget.h"
#include "Model/XYModel/linemodel.h"
#include "View/CentralWidget/ChartWidget/XYChartWidget/barchartwidget.h"
#include "Model/XYModel/barmodel.h"
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

QWidget* View::initialCentralWidget() const
{
    QWidget *w = new QWidget();
    QLabel *initial_text = new QLabel("Manage your chart with ChartManager");
    QFont f;
    f.setBold(true);
    f.setPointSize(36);
    initial_text->setStyleSheet("color:#d6d6d6");
    initial_text->setFont(f);
    initial_text->setAlignment(Qt::AlignCenter);
    QHBoxLayout *l = new QHBoxLayout(w);
    l->addWidget(initial_text);
    return w;
}

void View::configToolBarItems() const{

    tool_bar->setIconSize(QSize(36, 36));
    QAction *open_from_file = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\open.png"), "open from file");
    QAction *new_pie_chart = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\pie.png"), "new pie chart");
    QAction *new_line_chart = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\line.png"), "new line chart");
    QAction *new_bar_chart = tool_bar->addAction(QIcon("..\\Chart-Application\\icon\\bar.png"), "new bar chart");
    connectToolBarSignalsToSlots(new_pie_chart, new_bar_chart, new_line_chart, open_from_file);

}

View::View(QObject *parent) : QObject(parent), mainW(new QMainWindow()), tool_bar(new QToolBar()){
    configToolBarItems();
    mainW->setCentralWidget(initialCentralWidget());
    mainW->addToolBar(Qt::LeftToolBarArea, tool_bar);
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



