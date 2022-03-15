#include "chart.h"

Chart::Chart(const QString &t) : title(t){}

void Chart::changeTitle(const QString &t){
    title = t;
}

QJsonObject* Chart::parsing() const{
    QJsonObject *obj = new QJsonObject();
    obj->insert("title", title);
    return obj;
}
