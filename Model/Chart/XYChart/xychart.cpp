#include "xychart.h"

XYChart::XYChart(const QJsonObject &obj) : Chart(obj){
    if(!obj.value("x axis title").isUndefined())
        x_axis_title = obj.value("x axis title").toString();
    if(!obj.value("y axis title").isUndefined())
        y_axis_title = obj.value("y axis title").toString();
}

XYChart::XYChart(const QString& t) : Chart(t){}

QString XYChart::getXAxisTitle() const{
    return x_axis_title;
}

QString XYChart::getYAxisTitle() const{
    return y_axis_title;
}

void XYChart::changeXAxisTitle(const QString &new_name){
    x_axis_title = new_name;
}

void XYChart::changeYAxisTitle(const QString &new_name){
    y_axis_title = new_name;
}

QJsonObject* XYChart::parsing() const{
    QJsonObject *obj = Chart::parsing();
    obj->insert("x axis title", x_axis_title);
    obj->insert("y axis title", y_axis_title);
    return obj;
}
