#include "linechartwidget.h"
#include <QSortFilterProxyModel>
#include <QDoubleValidator>
#include "Model/linemodel.h"

void LineChartWidget::connectLineModelSignals() const{
    LineModel *line_model = dynamic_cast<LineModel*>(model);
    if(line_model){
        QObject::connect(line_model, SIGNAL(insertMultipleLines(int,int)), this, SLOT(multipleLinesInserted(int,int)));
        QObject::connect(line_model, SIGNAL(removeMultipleLines(int,int)), this, SLOT(multipleLinesRemoved(int,int)));
        QObject::connect(line_model, SIGNAL(lineAtChangedName(int,QString)), this, SLOT(lineAtChangedName(int,QString)));
        QObject::connect(line_model, SIGNAL(insertMultiplePointsAtLine(int,int,int)), this, SLOT(multiplePointsAtLineInserted(int,int,int)));
        QObject::connect(line_model, SIGNAL(removeMultiplePointsAtLine(int,int,int)), this, SLOT(multiplePointsAtLineRemoved(int,int,int)));
        QObject::connect(line_model, SIGNAL(pointAtLineChanged(int,int,double,double)), this, SLOT(pointAtLineChanged(int,int,double,double)));
    }
}

LineChartWidget::LineChartWidget(View *v, Model *m, QWidget *parent) : ChartWidget(v, m, parent), line_name(new QLineEdit()), point_info(new QGroupBox("Point")), point_info_layout(new QFormLayout(point_info)), points(new QComboBox()), point_x_value(new QLineEdit()), point_y_value(new QLineEdit()), insert_point(new QPushButton("Add Point")), remove_point(new QPushButton("Remove Point")){

    series->setModel(model);
    points->setModel(model);
    chart_info->setTitle("Line Chart");
    serie_info->setTitle("Line");
    add_serie->setText("Add Line");
    remove_serie->setText("Remove Line");
    save_chart->setText("Save Line Chart");
    save_chart_as->setText("Save Line Chart as");
    line_name->setPlaceholderText("<none>");

    serie_info_layout->addRow("Line name", line_name);
    serie_info_layout->addRow(point_info);
    point_info_layout->addRow("Points", points);
    point_info_layout->addRow("X value", point_x_value);
    point_info_layout->addRow("Y value", point_y_value);
    point_info_layout->addRow("Add Point", insert_point);
    point_info_layout->addRow("Remove Point", remove_point);
    point_x_value->setPlaceholderText("0");
    point_y_value->setPlaceholderText("0");
    QDoubleValidator *double_val = new QDoubleValidator(this);
    double_val->setDecimals(6);
    point_x_value->setValidator(double_val);
    point_y_value->setValidator(double_val);

    QObject::connect(series, SIGNAL(currentIndexChanged(int)), this, SLOT(currentLine(int)));
    QObject::connect(points, SIGNAL(currentIndexChanged(int)), this, SLOT(currentPoint(int)));
    QObject::connect(add_serie, SIGNAL(clicked()), this, SLOT(userInsertLine()));
    QObject::connect(remove_serie, SIGNAL(clicked()), this, SLOT(userRemoveLine()));
    QObject::connect(insert_point, SIGNAL(clicked()), this, SLOT(userInsertPoint()));
    QObject::connect(remove_point, SIGNAL(clicked()), this, SLOT(userRemovePoint()));
    QObject::connect(line_name, SIGNAL(editingFinished()), this, SLOT(userChangeLineName()));
    QObject::connect(point_x_value, SIGNAL(editingFinished()), this, SLOT(userChangeXPointValue()));
    QObject::connect(point_y_value, SIGNAL(editingFinished()), this, SLOT(userChangeYPointValue()));

    connectLineModelSignals();

}

void LineChartWidget::currentLine(int row){
    if(row != -1){
        current_line_index = model->index(row, 0);
        line_name->setText(model->data(current_line_index).toString());
        points->setRootModelIndex(current_line_index);
        points->setModelColumn(2);
    }
}

void LineChartWidget::userInsertLine(){
    int row = series->currentIndex();
    model->insertRows((row == -1) ? 0 : row, 1);
    points->setModelColumn(2);
}

void LineChartWidget::userRemoveLine(){
    int row = series->currentIndex();
    if(row != -1){
        model->removeRows(row, 1);
        current_line_index = QModelIndex();
    }
}

void LineChartWidget::userChangeLineName(){
    if(current_line_index.isValid())
        model->setData(current_line_index, line_name->text());
}

void LineChartWidget::currentPoint(int index){
    point_x_value->setText(model->data(model->index(points->currentIndex(), 0, current_line_index)).toString());
    point_y_value->setText(model->data(model->index(points->currentIndex(), 1, current_line_index)).toString());
}

void LineChartWidget::lineAtChangedName(int line_row, const QString &new_name){
    static_cast<QLineSeries*>(lines[line_row])->setName(new_name);
}

void LineChartWidget::userInsertPoint(){
    if(current_line_index.isValid()){
        int row = points->currentIndex();
        model->insertRows((row == -1) ? 0 : row, 1, current_line_index);
    }
}

void LineChartWidget::userRemovePoint(){
    if(current_line_index.isValid() && points->currentIndex() != -1)
    model->removeRows(points->currentIndex(), 1, current_line_index);
}

void LineChartWidget::userChangeXPointValue(){
    int row = points->currentIndex();
    if(current_line_index.isValid() && row != -1)
        model->setData(model->index(row, 0, current_line_index), point_x_value->text().toDouble());
}

void LineChartWidget::userChangeYPointValue(){
    int row = points->currentIndex();
    if(current_line_index.isValid() && row != -1)
        model->setData(model->index(points->currentIndex(), 1, current_line_index), point_y_value->text().toDouble());
}

void LineChartWidget::multipleLinesInserted(int row, int count){
    for(int i = 0; i < count; ++i){
        QLineSeries *new_line_series = new QLineSeries();
        lines.insert(row+i, new_line_series);
        chart->addSeries(new_line_series);
    }
}

void LineChartWidget::multipleLinesRemoved(int row, int count){
    for(int i = 0; i < count; ++i)
        chart->removeSeries(lines[row+i]);
    lines.erase(lines.begin()+row, lines.begin()+row+count);
}

void LineChartWidget::multiplePointsAtLineInserted(int line_row, int point_row, int count){
    for(int i = 0; i < count; ++i)
        static_cast<QLineSeries*>(lines[line_row])->insert(point_row, QPointF(0, 0));
}

void LineChartWidget::multiplePointsAtLineRemoved(int line_row, int point_row, int count){
    for(int i = 0; i < count; ++i)
        static_cast<QLineSeries*>(lines[line_row])->removePoints(point_row, count);
}

void LineChartWidget::pointAtLineChanged(int line_row, int row, double new_x, double new_y){
    static_cast<QLineSeries*>(lines[line_row])->replace(row, new_x, new_y);
}
