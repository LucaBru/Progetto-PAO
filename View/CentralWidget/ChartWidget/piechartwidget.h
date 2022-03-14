#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H
#include "Model/piemodel.h"
#include "View/CentralWidget/ChartWidget/chartwidget.h"

class PieChartWidget : public ChartWidget{
    Q_OBJECT
protected:
    QPieSeries *slices;
    QLineEdit *slice_name;
    QLineEdit *slice_value;
    QLineEdit *slice_color;

    void insertInvalidSerieAt(int index, int count) override;
    void removeInvalidSerieAt(int index, int count) override;

public:
    PieChartWidget(View *v, Model *m, QWidget *parent =nullptr);
protected slots:
    void changeSliceName();
    void changeSliceValue();
    void changeSliceColor();
};

#endif // PIECHARTWIDGET_H
