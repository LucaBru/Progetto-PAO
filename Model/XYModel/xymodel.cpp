#include "xymodel.h"
#include "Model/Chart/XYChart/xychart.h"

XYModel::XYModel(View *v, Chart *c, QObject *p) : Model(v, c, p){}

QString XYModel::getXAxisTitle() const{
    if(dynamic_cast<XYChart*>(chart))
        return static_cast<XYChart*>(chart)->getXAxisTitle();
}

QString XYModel::getYAxisTitle() const{
    if(dynamic_cast<XYChart*>(chart))
        return static_cast<XYChart*>(chart)->getYAxisTitle();
}

void XYModel::changeXAxisTitle(const QString &new_title){
    if(dynamic_cast<XYChart*>(chart))
        static_cast<XYChart*>(chart)->changeXAxisTitle(new_title);
}

void XYModel::changeYAxisTitle(const QString &new_title){
    if(dynamic_cast<XYChart*>(chart))
        static_cast<XYChart*>(chart)->changeYAxisTitle(new_title);
}

