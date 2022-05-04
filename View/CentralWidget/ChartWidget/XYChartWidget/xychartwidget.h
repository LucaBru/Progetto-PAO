#ifndef XYCHARTWIDGET_H
#define XYCHARTWIDGET_H
#include "View/CentralWidget/ChartWidget/chartwidget.h"

class XYChartWidget : public ChartWidget{
    Q_OBJECT
private:
    void connectSignalsToSlots() const;
    void configXYChartWidgetItems();
protected:
    QLineEdit *x_axis_title, *y_axis_title;
    QAbstractAxis *x_axis, *y_axis;
public:
    XYChartWidget(QAbstractAxis *x, QAbstractAxis *y, View *v, Model * m, QWidget *parent =nullptr);
    void createChartFromModel() override;
private slots:
    void userChangeXAxisTitle();
    void userChangeYAxisTitle();
};

#endif // XYCHARTWIDGET_H
