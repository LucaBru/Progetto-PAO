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
protected:
    Model *model;
    QGroupBox *chart_info;
    QGroupBox *serie_info;
    QComboBox *series;
    QLineEdit *chart_title;
    QRadioButton *legend;
    QPushButton *add_serie;
    QPushButton *remove_serie;
    QFormLayout *chart_info_layout;
    QFormLayout *serie_info_layout;
    QChart *chart;
    QChartView *chart_view;

    virtual void insertInvalidSerieAt(int index, int count =1) =0; //aggiunge le serie aggiunte al model
    virtual void removeInvalidSerieAt(int index, int count =1) =0; //rimuove le serie non presenti nel model

public:
    ChartWidget(View *v, Model * m, QWidget *parent =nullptr);
private slots:
    void changeTitle();
    void insertSerie();
    void removeSerie();
};

#endif // CHARTWIDGET_H
