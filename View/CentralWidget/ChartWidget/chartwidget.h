#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H
#include "View/CentralWidget/centralwidget.h"
#include "Model/model.h"
#include "QtCharts/QtCharts"
#include <QAbstractItemModel>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>

class ChartWidget : public CentralWidget{
    Q_OBJECT
private:
    void connectSignalsAndSlot() const;
    void configChartWidgetItems();
protected:
    Model *model;
    QGridLayout *main_layout;
    QGroupBox *chart_info;
    QGroupBox *serie_info;
    QComboBox *series;
    QLineEdit *chart_title;
    QPushButton *add_serie;
    QPushButton *remove_serie;
    QPushButton *save_chart;
    QPushButton *save_chart_as;
    QFormLayout *chart_info_layout;
    QFormLayout *serie_info_layout;
    QChart *chart;
    QChartView *chart_view;
    QString chart_file_path;

public:
    ChartWidget(View *v, Model * m, QWidget *parent =nullptr);
    void setCurrentChartPath(const QString& path);
    virtual void createChartFromModel();
    ~ChartWidget();
private slots:
    void changeTitle();
    void saveChart();
    void saveChartAs();
};

#endif // CHARTWIDGET_H
