#include "model.h"
#include "Model/Chart/PieChart/piechart.h"
#include "Model/Chart/XYChart/LineChart/linechart.h"
#include "Model/Chart/XYChart/BarChart/barchart.h"
#include <QFile>
#include <QIODevice>


Model::Model(View *v, Chart *c, QObject *p) : QAbstractItemModel(p), view(v), chart(c){}

Model::~Model(){
    //il model viene distrutto alla distruzione dalla vista => basta delete su chart
    //spiegazione: quando chuido un certo ChartWidget e ritorno così al menu principale elimino anche il model
    //non ha senso eliminare il model e non la vista e viceversa
    delete chart;
}

const QString& Model::getChartTitle() const{
    return chart->getTitle();
}

void Model::changeChartTitle(const QString &new_title){
    chart->changeTitle(new_title);
}

bool Model::save(const QString &path) const{
    QJsonObject *chart_obj = chart->parsing();

    QFile file(path);
    if(file.open(QFile::WriteOnly)){
        file.write(QJsonDocument(*chart_obj).toJson());
        delete chart_obj;
        return true;
    }
    return false;
}


























