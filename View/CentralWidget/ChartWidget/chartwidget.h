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
    QPushButton *save_chart;
    QPushButton *save_chart_as;
    QFormLayout *chart_info_layout;
    QFormLayout *serie_info_layout;
    QChart *chart;
    QChartView *chart_view;
    QString chart_file_path;

    virtual void insertInvalidSerieAt(int index, int count =1) =0; //aggiunge le serie aggiunte al model
    virtual void removeInvalidSerieAt(int index, int count =1) =0; //rimuove le serie non presenti nel model
    void alert(const QString& subtitle ="something goes wrong", const QString& title = "error", QMessageBox::Icon icon =QMessageBox::Critical) const; //funzione per mostrare un QMessageBox d'errore

public:
    ChartWidget(View *v, Model * m, QWidget *parent =nullptr);
private slots:
    void changeTitle();
    void insertSerie();
    void removeSerie();
    void saveChart();
    void saveChartAs();
};

#endif // CHARTWIDGET_H
