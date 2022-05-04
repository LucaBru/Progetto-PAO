#ifndef XYCHART_H
#define XYCHART_H
#include "Model/Chart/chart.h"

class XYChart : public Chart{
protected:
    QString x_axis_title, y_axis_title;
public:
    XYChart(const QJsonObject& obj);
    XYChart(const QString& title =QString());
    QString getXAxisTitle() const;
    QString getYAxisTitle() const;
    void changeXAxisTitle(const QString& new_name);
    void changeYAxisTitle(const QString& new_name);
    QJsonObject* parsing() const override;
};

#endif // XYCHART_H
