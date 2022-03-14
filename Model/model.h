#ifndef MODEL_H
#define MODEL_H
#include <QAbstractItemModel>
#include "Model/Chart/chart.h"
#include "View/view.h"

class View;

class Model : public QAbstractItemModel{
protected:
    View *view;
public:
    Model(View* v, QObject* p =nullptr);
    virtual void changeChartTitle(const QString& newTitle) =0;
    virtual void getChartFromDocument(const QJsonDocument& doc) =0;
    virtual void save(const QString& path) const =0;
};

#endif // MODEL_H
