#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H
#include "Model/piemodel.h"
#include "View/CentralWidget/ChartWidget/chartwidget.h"

class PieChartWidget : public ChartWidget{
    Q_OBJECT
private:
    void connectSignalsToSlots() const;
    void connectPieModelSignalsToSlots() const;
    void configChartWidgetItems() const;
    void configPieChartWidgetItems() const;

    void getSlicesFromModel();

protected:
    QPieSeries *slices;
    QLineEdit *slice_name;
    QLineEdit *slice_value;

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
    void sliceClicked(QPieSlice* slice);
};

#endif // PIECHARTWIDGET_H
