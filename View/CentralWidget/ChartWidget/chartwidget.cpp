#include "chartwidget.h"
#include <QGridLayout>
#include "Model/piemodel.h"


ChartWidget::ChartWidget(View *v, Model *m, QWidget *parent) : CentralWidget(v, parent), model(m), chart_info(new QGroupBox()), serie_info(new QGroupBox()), series(new QComboBox()), chart_title(new QLineEdit()), legend(new QRadioButton()), add_serie(new QPushButton()), remove_serie(new QPushButton()), chart(new QChart()), chart_view(new QChartView(chart)), chart_info_layout(new QFormLayout(chart_info)), serie_info_layout(new QFormLayout(serie_info)){
    series->setModel(model);
    QGridLayout *l = new QGridLayout(this);
    l->addWidget(chart_info, 0, 0);
    l->addWidget(serie_info, 1, 0);
    l->addWidget(chart_view, 0, 1, 2, 1);
    chart_title->setPlaceholderText("chart's title");
    chart_info_layout->addRow("Title", chart_title);
    chart_info_layout->addRow("Legend", legend);
    chart_info_layout->addRow("Series", series);
    chart_info_layout->addRow(add_serie);
    chart_info_layout->addRow(remove_serie);

    QObject::connect(chart_title, SIGNAL(editingFinished()), this, SLOT(changeTitle()));
    QObject::connect(add_serie, SIGNAL(clicked()), this, SLOT(insertSerie()));
    QObject::connect(remove_serie, SIGNAL(clicked()), this, SLOT(removeSerie()));
}

void ChartWidget::changeTitle(){
   model->changeChartTitle(chart_title->text());
}

void ChartWidget::insertSerie(){
    int current_index = series->currentIndex();
    if(model->insertRows((current_index == -1) ? 0 : current_index, 1, QModelIndex()))
        insertInvalidSerieAt(current_index);
}

void ChartWidget::removeSerie(){
    int current_index = series->currentIndex();
    if(model->removeRows((current_index == -1) ? 0 : current_index, 1, QModelIndex()))
        removeInvalidSerieAt(current_index);
}


