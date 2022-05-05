#include "linechartwidget.h"
#include <QSortFilterProxyModel>
#include <QDoubleValidator>
#include <QLocale>
#include "Model/Chart/XYChart/LineChart/linechart.h"
#include "Model/XYModel/linemodel.h"

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

void LineChartWidget::connectSignalsToSlots() const{
    QObject::connect(color, SIGNAL(clicked(bool)), this, SLOT(changeLineColor()));
    QObject::connect(series, SIGNAL(currentIndexChanged(int)), this, SLOT(currentLine(int)));
    QObject::connect(points, SIGNAL(currentIndexChanged(int)), this, SLOT(currentPoint(int)));
    QObject::connect(add_serie, SIGNAL(clicked()), this, SLOT(userInsertLine()));
    QObject::connect(remove_serie, SIGNAL(clicked()), this, SLOT(userRemoveLine()));
    QObject::connect(insert_point, SIGNAL(clicked()), this, SLOT(userInsertPoint()));
    QObject::connect(remove_point, SIGNAL(clicked()), this, SLOT(userRemovePoint()));
    QObject::connect(line_name, SIGNAL(returnPressed()), this, SLOT(userChangeLineName()));
    QObject::connect(point_x_value, SIGNAL(returnPressed()), this, SLOT(userChangePointValue()));
    QObject::connect(point_y_value, SIGNAL(returnPressed()), this, SLOT(userChangePointValue()));
    QObject::connect(remove_all_points, SIGNAL(clicked(bool)), this, SLOT(userRemoveAllPoints()));

    connectLineModelSignals();
}

void LineChartWidget::configChartWidgetItems() const{
    chart_info->setTitle("Line Chart");
    serie_info->setTitle("Line");
    add_serie->setText("Add Line");
    remove_serie->setText("Remove Line");
    chart_info_layout->insertRow(1, "Lines", series);
}

void LineChartWidget::configLineChartWidgetItems() const{
    QHBoxLayout *line_name_layout = new QHBoxLayout();
    line_name_layout->addWidget(line_name);
    line_name_layout->addWidget(color);
    serie_info_layout->addRow("Line name", line_name_layout);
    serie_info_layout->addRow(point_info);
    point_info_layout->addRow("Points", points);
    point_info_layout->addRow("X value", point_x_value);
    point_info_layout->addRow("Y value", point_y_value);
    point_info_layout->addRow("Add Point", insert_point);
    point_info_layout->addRow("Remove Point", remove_point);
    point_info_layout->addRow("Remove All Points", remove_all_points);
    point_x_value->setPlaceholderText("");
    point_y_value->setPlaceholderText("0");
    QDoubleValidator *double_val = new QDoubleValidator(const_cast<LineChartWidget*>(this));
    double_val->setDecimals(6);
    point_x_value->setValidator(double_val);
    point_y_value->setValidator(double_val);
}

void LineChartWidget::configInitialQLineSeries(){
    int lines_count = model->rowCount();
    for(int i = 0; i < lines_count; ++i){
        QLineSeries * new_line = new QLineSeries();
        chart->addSeries(new_line);
        QModelIndex new_line_model_index = model->index(i, 0);
        new_line->setName(model->data(new_line_model_index).toString());
        new_line->setColor(model->data(new_line_model_index, Qt::DecorationRole).value<QColor>());
        new_line->attachAxis(x_axis);
        new_line->attachAxis(y_axis);
        lines.push_back(new_line);
        configInitialQPointFsInQLineSeries(new_line_model_index);
    }
}

void LineChartWidget::configInitialQPointFsInQLineSeries(const QModelIndex& line_index){
    if(line_index.isValid()){
        int points_count = model->rowCount(line_index);
        for(int i = 0; i < points_count; ++i){
            double xvalue = model->data(model->index(i, 0, line_index)).toDouble();
            double yvalue = model->data(model->index(i, 1, line_index)).toDouble();
            updateAxisRangeValue(xvalue, yvalue);
            lines[line_index.row()]->append(QPointF(xvalue, yvalue));
        }
    }
}

void LineChartWidget::updateAxisRangeValue(double point_x, double point_y){
    QValueAxis *x = static_cast<QValueAxis*>(x_axis);
    QValueAxis *y = static_cast<QValueAxis*>(y_axis);
    if(point_x < x->min() || point_x > x->max()){
        if(point_x > x->max())
            x->setMax(point_x);
        else
            x->setMin(point_x);
    }
    if(point_y < y->min() || point_y > y->max()){
        if(point_y > y->max())
            y->setMax(point_y);
        else
            y->setMin(point_y);
    }
}

LineChartWidget::LineChartWidget(View *v, Model *m, QWidget *parent) : XYChartWidget(new QValueAxis(), new QValueAxis(), v, m, parent), line_name(new QLineEdit()), point_info(new QGroupBox("Point")), point_info_layout(new QFormLayout(point_info)), points(new QComboBox()), point_x_value(new QLineEdit()), point_y_value(new QLineEdit()), insert_point(new QPushButton("Add Point")), remove_point(new QPushButton("Remove Point")), remove_all_points(new QPushButton("Remove All Points")){
    y_axis->setRange(0, 1);
    x_axis->setRange(0, 1);
    configChartWidgetItems();
    configLineChartWidgetItems();
    connectSignalsToSlots();
}

void LineChartWidget::createChartFromModel(){
    XYChartWidget::createChartFromModel();
    configInitialQLineSeries();
    series->setCurrentIndex(-1);
}

void LineChartWidget::currentLine(int row){
    if(row != -1){
        current_line_index = model->index(row, 0);
        line_name->setText(series->currentText());
        points->setModel(model);
        points->setRootModelIndex(current_line_index);
        points->setModelColumn(2);
        points->setCurrentIndex(-1);
    }
    else
        current_line_index = QModelIndex();
    points->setCurrentIndex(-1);
    line_name->setStyleSheet("");
}

void LineChartWidget::userInsertLine(){
    int row = series->currentIndex()+1;
    model->insertRows(row, 1);
    series->setCurrentIndex(row);
}

void LineChartWidget::userRemoveLine(){
    int row = series->currentIndex();
    if(row != -1){
        line_name->setText("");
        model->removeRows(row, 1);
        point_x_value->setText("");
        point_y_value->setText("");
    }
}

void LineChartWidget::userChangeLineName(){
    if(series->currentIndex() != -1){
        bool result = model->setData(model->index(series->currentIndex(), 0), line_name->text());
        if(!result){
            QMessageBox::warning(this, "Change Line Name", "Something goes wrong, rember that the name must by unique");
            line_name->setStyleSheet("border: 1px solid red");
        }
        else
            line_name->setStyleSheet("");
    }
}

void LineChartWidget::currentPoint(int index){
    if(index != -1 && current_line_index.isValid()){
        point_x_value->setText(model->data(model->index(points->currentIndex(), 0, current_line_index)).toString());
        point_y_value->setText(model->data(model->index(points->currentIndex(), 1, current_line_index)).toString());
    }
    else{
        point_x_value->setText("");
        point_y_value->setText("");
    }
}

void LineChartWidget::lineAtChangedName(int line_row, const QString &new_name){
   lines[line_row]->setName(new_name);
}

void LineChartWidget::userInsertPoint(){
    if(current_line_index.isValid()){
        int row = points->currentIndex()+1;
        model->insertRows(row, 1, current_line_index);
        points->setCurrentIndex(row);
    }
}

void LineChartWidget::userRemovePoint(){
    if(current_line_index.isValid() && points->currentIndex() != -1)
        model->removeRows(points->currentIndex(), 1, current_line_index);
}

void LineChartWidget::userRemoveAllPoints(){
        if(current_line_index.isValid() && series->currentIndex() != -1 && points->count() > 0){
        QMessageBox *user_confirm = new QMessageBox();
        user_confirm->setText("Are you sure you want to delete all points of line '"+series->currentText()+"' ?");
        user_confirm->setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes | QMessageBox::No);
        user_confirm->setDefaultButton(QMessageBox::No);
        int ret = user_confirm->exec();
        if(ret == QMessageBox::Yes)
         model->removeRows(0, points->count(), current_line_index);
        }
}

void LineChartWidget::userChangePointValue(){
    int row = points->currentIndex();
    if(current_line_index.isValid() && row != -1){
        double x_val = point_x_value->text().toDouble();
        double y_val = point_y_value->text().toDouble();
        model->setData(model->index(row, 0, current_line_index), x_val);
        model->setData(model->index(row, 1, current_line_index), y_val);
    }
}


void LineChartWidget::multipleLinesInserted(int row, int count){
    for(int i = 0; i < count; ++i){
        QLineSeries *new_line_series = new QLineSeries();
        chart->addSeries(new_line_series);
        new_line_series->setColor(model->data(model->index(row, 0), Qt::DecorationRole).value<QColor>());
        new_line_series->attachAxis(x_axis);
        new_line_series->attachAxis(y_axis);
        lines.insert(row+i, new_line_series);
    }
}

void LineChartWidget::multipleLinesRemoved(int row, int count){
    for(int i = 0; i < count; ++i)
        chart->removeSeries(lines[row+i]);
    lines.erase(lines.begin()+row, lines.begin()+row+count);
}

void LineChartWidget::multiplePointsAtLineInserted(int line_row, int point_row, int count){
    int default_value_from_index = -1;
    int points_size = lines[line_row]->count();
    if(points_size > 0){
        if(point_row == 0)
            default_value_from_index = 1;
        else
            default_value_from_index = point_row-1;
    }

    double x_default_value = (default_value_from_index == -1) ? 0 : lines[line_row]->at(default_value_from_index).x();
    double y_default_value = (default_value_from_index == -1) ? 0 : lines[line_row]->at(default_value_from_index).y();
    updateAxisRangeValue(x_default_value, y_default_value);
    for(int i = 0; i < count; ++i)
        lines[line_row]->insert(point_row+i, QPointF(x_default_value, y_default_value));
}

void LineChartWidget::multiplePointsAtLineRemoved(int line_row, int point_row, int count){
    lines[line_row]->removePoints(point_row, count);
}

void LineChartWidget::pointAtLineChanged(int line_row, int row, double new_x, double new_y){
    lines[line_row]->replace(row, new_x, new_y);
    updateAxisRangeValue(new_x, new_y);
}


void LineChartWidget::changeLineColor(){
    int index = series->currentIndex();
    if(index != -1){
        QColor new_color = QColorDialog::getColor(lines[index]->color(), this, "Choose new line color");
        if(new_color.isValid()){
            if(model->setData(model->index(index, 0), new_color, Qt::DecorationRole))
                lines[index]->setColor(new_color);
        }
    }
}
