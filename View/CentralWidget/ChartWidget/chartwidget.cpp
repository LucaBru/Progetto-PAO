#include "chartwidget.h"
#include <QGridLayout>
#include "Model/piemodel.h"

void ChartWidget::connectSignalsToSlots() const{
    QObject::connect(chart_title, SIGNAL(returnPressed()), this, SLOT(changeTitle()));
    QObject::connect(save, SIGNAL(triggered(bool)), this, SLOT(saveChart()));
    QObject::connect(save_as, SIGNAL(triggered(bool)), this, SLOT(saveChartAs()));
}

void ChartWidget::configChartWidgetItems(){
    QFont f;
    f.setBold(true);
    f.setPointSize(18);
    chart->setTitleFont(f);
    main_layout->addWidget(chart_info, 0, 0);
    main_layout->addWidget(serie_info, 1, 0);
    main_layout->addWidget(chart_view, 0, 1, 2, 1);
    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 2);
    chart_title->setPlaceholderText("chart's title");
    chart_info_layout->addRow("Title", chart_title);
    chart_info_layout->addRow(add_serie);
    chart_info_layout->addRow(remove_serie);
    color->setIcon(QIcon(":/icon/color.png"));
}

ChartWidget::ChartWidget(View *v, Model *m, QWidget *parent) : CentralWidget(v, parent), model(m), chart_info(new QGroupBox()), serie_info(new QGroupBox()), series(new QComboBox()), chart_title(new QLineEdit()), add_serie(new QPushButton()), remove_serie(new QPushButton()), chart(new QChart()), chart_view(new QChartView(chart)), chart_info_layout(new QFormLayout(chart_info)), serie_info_layout(new QFormLayout(serie_info)), main_layout(new QGridLayout(this)), color(new QToolButton()){
    chart_view->setRenderHint(QPainter::Antialiasing);
    series->setModel(model);
    configChartWidgetItems();
    save = view->addActionToToolBar(QIcon(":/icon/save.png"), "save");
    save_as = view->addActionToToolBar(QIcon(":/icon/save as.png"), "save as");
    connectSignalsToSlots();
}

void ChartWidget::createChartFromModel(){
    QString title = model->getChartTitle();
    chart->setTitle(title);
    chart_title->setText(title);
}

ChartWidget::~ChartWidget(){
    //tutti gli items eliminati
    //chart essendo associato a chart_view viene eliminato da esso
    view->removeAction(save);
    view->removeAction(save_as);
    delete save;
    delete save_as;
    delete model;
}


void ChartWidget::changeTitle(){
    QString title = chart_title->text();
    chart->setTitle(title);
    model->changeChartTitle(title);
}

void ChartWidget::updateFilePath(const QString &path){
    chart_file_path = path;
}


void ChartWidget::saveChart(){
    if(chart_file_path.isEmpty())
        saveChartAs();
    else
        if(!model->save(chart_file_path))
            QMessageBox::warning(this, "Warning", "something goes wrong, try again!");
}

void ChartWidget::saveChartAs(){
   chart_file_path = QFileDialog::getSaveFileName(this, tr("Save Chart"), "", tr("(*.json)"));
   if(!chart_file_path.isEmpty() && !model->save(chart_file_path)){
       QMessageBox::warning(this, "Warning", "something goes wrong, try again!");
       chart_file_path = "";
   }
}




