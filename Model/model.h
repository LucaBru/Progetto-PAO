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
    const QString& getChartTitle() const;
    virtual void changeChartTitle(const QString& newTitle);
    virtual bool save(const QString& path) const;
};

#endif // MODEL_H

/*
 * i vari model devono avvisare la vista degli aggiornamenti fatti
 *
 */
