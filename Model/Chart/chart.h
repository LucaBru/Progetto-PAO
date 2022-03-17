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
    Chart(const QJsonObject& obj);
    const QString& getTitle() const;
    void changeTitle(const QString& newTitle);
    virtual QJsonObject* parsing() const;
};

#endif // CHART_H
