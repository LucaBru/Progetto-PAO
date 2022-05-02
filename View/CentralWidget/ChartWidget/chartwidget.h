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
#include <QToolButton>
#include <QAction>

class ChartWidget : public CentralWidget{
    Q_OBJECT
private:
    void connectSignalsToSlot() const;
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
    QRadioButton *legend;
    QToolButton *color;
    QAction *save;
    QAction *save_as;
    QFormLayout *chart_info_layout;
    QFormLayout *serie_info_layout;
    QChart *chart;
    QChartView *chart_view;
    QString chart_file_path;

public:
    ChartWidget(View *v, Model * m, QWidget *parent =nullptr);
    virtual void createChartFromModel();
    void updateFilePath(const QString& path);
    ~ChartWidget();
private slots:
    void changeTitle();
public slots:
    void saveChart();
    void saveChartAs();
};

#endif // CHARTWIDGET_H
