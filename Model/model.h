#ifndef MODEL_H
#define MODEL_H
#include <QAbstractItemModel>
#include "Model/Chart/chart.h"
#include "View/view.h"

class View;

class Model : public QAbstractItemModel{
protected:
    View *view;
    Chart *chart;
public:
    Model(View* v, Chart* c, QObject* p =nullptr);
    virtual void changeChartTitle(const QString& newTitle);
    virtual void getChartFromDocument(const QJsonDocument& doc) =0;
    virtual bool save(const QString& path) const;
};

#endif // MODEL_H
