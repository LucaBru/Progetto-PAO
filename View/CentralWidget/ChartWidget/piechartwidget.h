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

    void connectPieModelSignals() const;
public:
    PieChartWidget(View *v, Model *m, QWidget *parent =nullptr);
    void createChartFromModel() override;
protected slots:
    void userChangeSliceName();
    void userChangeSliceValue();
    void userInsertSlice();
    void userRemoveSlice();
    void currentSlice(int index);
    void multipleSlicesInserted(int first, int count);
    void multipleSlicesRemoved(int first, int count);
    void sliceChangedName(int index, const QString& name);
    void sliceChangedValue(int index, double value);
};

#endif // PIECHARTWIDGET_H
