#include "chartwidget.h"
#include <QGridLayout>
#include "Model/piemodel.h"

ChartWidget::ChartWidget(View *v, Model *m, QWidget *parent) : CentralWidget(v, parent), model(m), chart_info(new QGroupBox()), serie_info(new QGroupBox()), series(new QComboBox()), chart_title(new QLineEdit()), add_serie(new QPushButton()), remove_serie(new QPushButton()), save_chart(new QPushButton()), save_chart_as(new QPushButton()), chart(new QChart()), chart_view(new QChartView(chart)), chart_info_layout(new QFormLayout(chart_info)), serie_info_layout(new QFormLayout(serie_info)){

    chart_title->setText(model->getChartTitle());
    //series->setModel(model);
    QGridLayout *l = new QGridLayout(this);
    l->addWidget(chart_info, 0, 0);
    l->addWidget(serie_info, 1, 0);
    l->addWidget(chart_view, 0, 1, 2, 1);
    chart_title->setPlaceholderText("chart's title");
    chart_info_layout->addRow("Title", chart_title);
    chart_info_layout->addRow("Series", series);
    chart_info_layout->addRow(add_serie);
    chart_info_layout->addRow(remove_serie);
    chart_info_layout->addRow(save_chart);
    chart_info_layout->addRow(save_chart_as);

    QObject::connect(chart_title, SIGNAL(editingFinished()), this, SLOT(changeTitle()));
    QObject::connect(save_chart, SIGNAL(clicked()), this, SLOT(saveChart()));
    QObject::connect(save_chart_as, SIGNAL(clicked()), this, SLOT(saveChartAs()));

}

ChartWidget::~ChartWidget(){
    //tutti gli items eliminati
    //chart essendo associato a chart_view viene eliminato da esse
    delete model;
}

void ChartWidget::setCurrentChartPath(const QString &path){
    chart_file_path = path;
}

void ChartWidget::changeTitle(){
    QString title = chart_title->text();
    chart->setTitle(title);
    model->changeChartTitle(title);
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
   if(!model->save(chart_file_path)){
       QMessageBox::warning(this, "Warning", "something goes wrong, try again!");
       chart_file_path = "";
   }
}

