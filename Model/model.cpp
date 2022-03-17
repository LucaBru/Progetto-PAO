#include "model.h"
#include "Model/Chart/PieChart/piechart.h"
#include "Model/Chart/LineChart/linechart.h"
#include "Model/Chart/BarChart/barchart.h"
#include <QFile>
#include <QIODevice>


Model::Model(View *v, Chart *c, QObject *p) : QAbstractItemModel(p), view(v), chart(c){}

const QString& Model::getChartTitle() const{
    return chart->getTitle();
}

void Model::changeChartTitle(const QString &new_title){
    chart->changeTitle(new_title);
}

bool Model::save(const QString &path) const{
    QJsonObject *chart_obj = chart->parsing(); //rivedere

    QFile file(path);
    if(file.open(QFile::WriteOnly)){
        file.write(QJsonDocument(*chart_obj).toJson());
        delete chart_obj;
        return true;
    }
    return false;
}

/*
 * una volta fatto ciò perdere un po di tempo a riorganizzare graficamente l'intera struttura (altrimenti vado a rischio di non capirmi più fra un paio di ore !!)
 *
 */

























