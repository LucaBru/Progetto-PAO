#include "piechartwidget.h"
#include <QDoubleValidator>

void PieChartWidget::connectPieModelSignalsToSlots() const{
    if(dynamic_cast<PieModel*>(model)){
        PieModel *pie_model = static_cast<PieModel*>(model);
        QObject::connect(pie_model, SIGNAL(addMultipleSlices(int,int)), this, SLOT(multipleSlicesInserted(int,int)));
        QObject::connect(pie_model, SIGNAL(removeMultipleSlices(int,int)), this, SLOT(multipleSlicesRemoved(int,int)));
        QObject::connect(pie_model, SIGNAL(sliceAtNameChanged(int,QString)), this, SLOT(sliceChangedName(int,QString)));
        QObject::connect(pie_model, SIGNAL(sliceAtValueChanged(int,double)), this, SLOT(sliceChangedValue(int,double)));
        QObject::connect(slices, SIGNAL(clicked(QPieSlice*)), this, SLOT(sliceClicked(QPieSlice*)));
    }
}

void PieChartWidget::connectSignalsToSlots() const{
    QObject::connect(add_serie, SIGNAL(clicked()), this, SLOT(userInsertSlice()));
    QObject::connect(remove_serie, SIGNAL(clicked()), this, SLOT(userRemoveSlice()));
    QObject::connect(slice_name, SIGNAL(returnPressed()), this, SLOT(userChangeSliceName()));
    QObject::connect(slice_value, SIGNAL(returnPressed()), this, SLOT(userChangeSliceValue()));
    QObject::connect(series, SIGNAL(currentIndexChanged(int)), this, SLOT(currentSlice(int)));
    QObject::connect(color, SIGNAL(clicked(bool)), this, SLOT(changeColor()));

}

void PieChartWidget::configChartWidgetItems(){
    chart->addSeries(slices);
    add_serie->setText("Add Slice");
    remove_serie->setText("Remove Slice");
    chart_info->setTitle("Pie Chart");
    serie_info->setTitle("Slice");
    chart_info_layout->insertRow(1, "Slices", series);
}

void PieChartWidget::configPieChartWidgetItems(){
    QHBoxLayout *slice_name_layout = new QHBoxLayout();
    slice_name_layout->addWidget(slice_name);
    slice_name_layout->addWidget(color);
    serie_info_layout->addRow("slice name", slice_name_layout);
    serie_info_layout->addRow("slice value", slice_value);
    slice_name->setPlaceholderText("none");
    slice_value->setValidator(new QDoubleValidator(const_cast<PieChartWidget*>(this)));
}

PieChartWidget::PieChartWidget(View *v, Model *m, QWidget *parent) : ChartWidget(v, m, parent), slice_name(new QLineEdit()), slice_value(new QLineEdit()), slices(new QPieSeries()){
    configChartWidgetItems();
    configPieChartWidgetItems();
    connectSignalsToSlots();
    connectPieModelSignalsToSlots();
}

void PieChartWidget::getSlicesFromModel(){
    int slices_count = model->rowCount();
    for(int i=0; i < slices_count; ++i){
        QModelIndex index = model->index(i, 0);
        QPieSlice *new_slice = new QPieSlice(model->data(index).toString(), model->data(model->index(i, 1)).toDouble());
        new_slice->setColor(model->data(index, Qt::DecorationRole).value<QColor>());
        slices->append(new_slice);
    }
}

void PieChartWidget::changeSliceColor(int slice_index){
    QPieSlice *slice = slices->slices()[slice_index];
    QColor new_color = QColorDialog::getColor(slice->color(), this, "Choose new slice color");
    if(new_color.isValid())
        if(model->setData(model->index(slice_index, 0), new_color, Qt::DecorationRole))
            slice->setColor(new_color);
}

void PieChartWidget::createChartFromModel(){
    ChartWidget::createChartFromModel();
    getSlicesFromModel();
    series->setCurrentIndex(-1);
}

void PieChartWidget::userChangeSliceName(){
    if(series->currentIndex() != -1){
        bool result = model->setData(model->index(series->currentIndex(), 0), slice_name->text());
        if(!result){
            QMessageBox::warning(this, "Change Slice Name", "Something goes wrong, rember that the name must by unique");
            slice_name->setStyleSheet("border: 1px solid red");
        }
    }
}

void PieChartWidget::userChangeSliceValue(){
    int current_index = series->currentIndex();
    if(current_index >= 0)
        model->setData(model->index(current_index, 1), slice_value->text());
}

void PieChartWidget::userInsertSlice(){
    int current_index = series->currentIndex()+1;
    model->insertRows(current_index, 1, QModelIndex());
    series->setCurrentIndex(current_index);
}

void PieChartWidget::userRemoveSlice() {
    int current_index = series->currentIndex();
    model->removeRows((current_index == -1) ? 0 : current_index, 1, QModelIndex());
}

void PieChartWidget::currentSlice(int index){
    slice_name->setStyleSheet("");
    slice_name->setText(model->data(model->index(index, 0, QModelIndex())).toString());
    slice_value->setText(model->data(model->index(index, 1, QModelIndex())).toString());
}

void PieChartWidget::multipleSlicesInserted(int first, int count){
    for(int i=0; i < count; ++i){
        QPieSlice *new_slice = new QPieSlice("", 0);
        new_slice->setColor(model->data(model->index(first+i, 0), Qt::DecorationRole).value<QColor>());
        slices->insert(first+i, new_slice);
    }
}

void PieChartWidget::multipleSlicesRemoved(int first, int count){
    QList<QPieSlice*> list = slices->slices();
    for(QList<QPieSlice*>::iterator i = list.begin()+first; count > 0; --count)
        slices->remove(*i);
}

void PieChartWidget::sliceChangedName(int index, const QString &name){
    slices->slices()[index]->setLabel(name);
    slice_name->setStyleSheet("");
}

void PieChartWidget::sliceChangedValue(int index, double value){
    QList<QPieSlice*> list = slices->slices();
    if(index >= 0 && index < list.size())
        list.takeAt(index)->setValue(value);
}

void PieChartWidget::sliceClicked(QPieSlice *slice){
    QList<QPieSlice*> slices_list = slices->slices();
    int slice_index = 0;
    for(QList<QPieSlice*>::const_iterator i = slices_list.begin(); i != slices_list.end() && *i != slice; ++i, ++slice_index);
    changeSliceColor(slice_index);
}

void PieChartWidget::changeColor(){
    int index = series->currentIndex();
    if(index != -1)
        changeSliceColor(index);
}



