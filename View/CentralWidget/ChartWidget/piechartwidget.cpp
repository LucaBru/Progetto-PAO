#include "piechartwidget.h"
#include <QDoubleValidator>

void PieChartWidget::insertInvalidSerieAt(int index, int count){
    for(int i=0; i<count; ++i)
        slices->insert(index+i, new QPieSlice("", 0.0));
}

void PieChartWidget::removeInvalidSerieAt(int index, int count){
    for(int i=0; i < count; ++i)
        slices->remove(slices->slices().takeAt(index));
}

PieChartWidget::PieChartWidget(View *v, Model *m, QWidget *parent) : ChartWidget(v, m, parent), slice_name(new QLineEdit()), slice_value(new QLineEdit()), slice_color(new QLineEdit()), slices(new QPieSeries()){

    chart->addSeries(slices);
    slice_name->setPlaceholderText("none");
    slice_value->setValidator(new QDoubleValidator(this));
    add_serie->setText("Add Slice");
    remove_serie->setText("Remove Slice");
    save_chart->setText("Save Pie Chart");
    save_chart_as->setText("Save Pie Chart as");
    chart_info->setTitle("Pie Chart");
    serie_info->setTitle("Slice");
    serie_info_layout->addRow("slice name", slice_name);
    serie_info_layout->addRow("slice value", slice_value);
    serie_info_layout->addRow("slice color", slice_color);

    QObject::connect(slice_name, SIGNAL(editingFinished()), this, SLOT(changeSliceName()));
    QObject::connect(slice_value, SIGNAL(editingFinished()), this, SLOT(changeSliceValue()));
    QObject::connect(series, SIGNAL(currentIndexChanged(int)), this, SLOT(currentSlice(int)));
}

void PieChartWidget::changeSliceName(){
    int current_index = series->currentIndex();
    if(current_index >= 0 && model->setData(model->index(current_index, 0), slice_name->text()));
}

void PieChartWidget::changeSliceValue(){
    int current_index = series->currentIndex();
    if(current_index >= 0 && model->setData(model->index(current_index, 1), slice_value->text()))
       slices->slices().takeAt(current_index)->setValue(slice_value->text().toDouble());
}
void PieChartWidget::changeSliceColor(){}

void PieChartWidget::currentSlice(int index){
    slice_name->setText(model->data(model->index(index, 0, QModelIndex())).toString());
    slice_value->setText(model->data(model->index(index, 1, QModelIndex())).toString());
}



