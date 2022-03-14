#ifndef CHART_H
#define CHART_H
#include <QString>
#include <QJsonDocument>

class Chart{
protected:
    QString title;
public:
    Chart(const QString& t =QString());
    void changeTitle(const QString& newTitle);
    virtual void getFromJSON(const QJsonDocument& doc) =0;
    virtual QJsonDocument parsing() const =0;
};

#endif // CHART_H
