#ifndef XYMODEL_H
#define XYMODEL_H
#include "Model/model.h"

class XYModel : public Model{
public:
    XYModel(View* v, Chart* c, QObject* p =nullptr);
    QString getXAxisTitle() const;
    QString getYAxisTitle() const;
    void changeXAxisTitle(const QString& new_title);
    void changeYAxisTitle(const QString& new_title);
};

#endif // XYMODEL_H
