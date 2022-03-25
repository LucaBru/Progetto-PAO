#ifndef CHART_H
#define CHART_H
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

class ChartData{
public:
    virtual ChartData* parentItem() const;
    virtual ~ChartData() =default;
};

class Chart{
protected:
    QString title;
public:
    Chart(const QString& t =QString());
    Chart(const QJsonObject& obj);
    virtual ~Chart() =default;
    const QString& getTitle() const;
    void changeTitle(const QString& newTitle);
    virtual QJsonObject* parsing() const;
};

#endif // CHART_H
