#ifndef CHART_H
#define CHART_H
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

class Chart{
protected:
    QString title;
public:
    Chart(const QString& t =QString());
    void changeTitle(const QString& newTitle);
    virtual void getFromJSON(const QJsonDocument& doc) =0;
    virtual QJsonObject* parsing() const;
};

#endif // CHART_H
