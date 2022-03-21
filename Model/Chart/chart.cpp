#include "chart.h"

ChartData* ChartData::parentItem() const{
    return nullptr;
}

Chart::Chart(const QString &t) : title(t){}

Chart::Chart(const QJsonObject &obj) : title((obj.value("title").isUndefined()) ? "" : obj.value("title").toString()){}

const QString& Chart::getTitle() const{
    return title;
}

void Chart::changeTitle(const QString &t){
    title = t;
}

QJsonObject* Chart::parsing() const{
    QJsonObject *obj = new QJsonObject();
    obj->insert("title", title);
    return obj;
}
