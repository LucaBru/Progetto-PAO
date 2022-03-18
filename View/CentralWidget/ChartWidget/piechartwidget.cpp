#include "piechartwidget.h"
#include <QDoubleValidator>

void PieChartWidget::connectPieModelSignals() const{
    if(dynamic_cast<PieModel*>(model)){
        PieModel *pie_model = static_cast<PieModel*>(model);
        QObject::connect(pie_model, SIGNAL(addMultipleSlices(int,int)), this, SLOT(multipleSlicesInserted(int,int)));
        QObject::connect(pie_model, SIGNAL(removeMultipleSlices(int,int)), this, SLOT(multipleSlicesRemoved(int,int)));
        QObject::connect(pie_model, SIGNAL(sliceAtNameChanged(int,QString)), this, SLOT(sliceChangedName(int,QString)));
        QObject::connect(pie_model, SIGNAL(sliceAtValueChanged(int,double)), this, SLOT(sliceChangedValue(int,double)));
    }
}

void PieChartWidget::checkInitialChartValues(){
    int slices_count = model->rowCount();
    for(int i=0; i < slices_count; ++i)
        slices->insert(i, new QPieSlice(series->itemText(i), model->data(model->index(i, 1)).toDouble()));
}

PieChartWidget::PieChartWidget(View *v, Model *m, QWidget *parent) : ChartWidget(v, m, parent), slice_name(new QLineEdit()), slice_value(new QLineEdit()), slice_color(new QLineEdit()), slices(new QPieSeries()){

    checkInitialChartValues();
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

    QObject::connect(add_serie, SIGNAL(clicked()), this, SLOT(userInsertSlice()));
    QObject::connect(remove_serie, SIGNAL(clicked()), this, SLOT(userRemoveSlice()));
    QObject::connect(slice_name, SIGNAL(editingFinished()), this, SLOT(userChangeSliceName()));
    QObject::connect(slice_value, SIGNAL(editingFinished()), this, SLOT(userChangeSliceValue()));
    QObject::connect(series, SIGNAL(currentIndexChanged(int)), this, SLOT(currentSlice(int)));

    connectPieModelSignals();
}

void PieChartWidget::userChangeSliceName(){
    int current_index = series->currentIndex();
    if(current_index >= 0)
        model->setData(model->index(current_index, 0), slice_name->text());
}

void PieChartWidget::userChangeSliceValue(){
    int current_index = series->currentIndex();
    if(current_index >= 0)
        model->setData(model->index(current_index, 1), slice_value->text());
}

void PieChartWidget::userInsertSlice(){
    int current_index = series->currentIndex();
    model->insertRows((current_index == -1) ? 0 : current_index, 1, QModelIndex());
}

void PieChartWidget::userRemoveSlice() {
    int current_index = series->currentIndex();
    model->removeRows((current_index == -1) ? 0 : current_index, 1, QModelIndex());
}

void PieChartWidget::currentSlice(int index){
    slice_name->setText(model->data(model->index(index, 0, QModelIndex())).toString());
    slice_value->setText(model->data(model->index(index, 1, QModelIndex())).toString());
}

void PieChartWidget::multipleSlicesInserted(int first, int count){
    for(int i=0; i < count; ++i)
        slices->insert(first+i, new QPieSlice("", 0));
}

void PieChartWidget::multipleSlicesRemoved(int first, int count){
    QList<QPieSlice*> list = slices->slices();
    for(QList<QPieSlice*>::iterator i = list.begin()+first; count > 0; --count)
        slices->remove(*i);
}

void PieChartWidget::sliceChangedName(int index, const QString &name){
    QList<QPieSlice*> list = slices->slices();
    if(index >= 0 && index < list.size())
        list[index]->setLabel(name);
}

void PieChartWidget::sliceChangedValue(int index, double value){
    QList<QPieSlice*> list = slices->slices();
    if(index >= 0 && index < list.size())
        list.takeAt(index)->setValue(value);
}



