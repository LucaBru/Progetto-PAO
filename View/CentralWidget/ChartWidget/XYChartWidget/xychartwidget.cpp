#include "xychartwidget.h"
#include "Model/XYModel/xymodel.h"

void XYChartWidget::connectSignalsToSlots() const{
    QObject::connect(x_axis_title, SIGNAL(returnPressed()), this, SLOT(userChangeXAxisTitle()));
    QObject::connect(y_axis_title, SIGNAL(returnPressed()), this, SLOT(userChangeYAxisTitle()));
}

void XYChartWidget::configXYChartWidgetItems(){
    chart_info_layout->addRow("X axis title", x_axis_title);
    chart_info_layout->addRow("Y axis title", y_axis_title);

}

XYChartWidget::XYChartWidget(QAbstractAxis *x, QAbstractAxis *y, View *v, Model *m, QWidget *parent) : ChartWidget(v, m, parent), x_axis(x), y_axis(y), x_axis_title(new QLineEdit()), y_axis_title(new QLineEdit()){
    connectSignalsToSlots();
    configXYChartWidgetItems();
    chart->addAxis(x_axis, Qt::AlignBottom);
    chart->addAxis(y_axis, Qt::AlignLeft);
}

void XYChartWidget::createChartFromModel(){
    ChartWidget::createChartFromModel();
    XYModel *m = dynamic_cast<XYModel*>(model);
    if(m){
        QString x_title = m->getXAxisTitle();
        QString y_title = m->getYAxisTitle();
        x_axis_title->setText(x_title);
        y_axis_title->setText(y_title);
        x_axis->setTitleText(x_title);
        y_axis->setTitleText(y_title);
    }
}

void XYChartWidget::userChangeXAxisTitle(){
    QString new_title = x_axis_title->text();
    if(dynamic_cast<XYModel*>(model))
        static_cast<XYModel*>(model)->changeXAxisTitle(new_title);
    x_axis->setTitleText(new_title);
}

void XYChartWidget::userChangeYAxisTitle(){
    QString new_title = y_axis_title->text();
    if(dynamic_cast<XYModel*>(model))
        static_cast<XYModel*>(model)->changeYAxisTitle(new_title);
    y_axis->setTitleText(new_title);
}
