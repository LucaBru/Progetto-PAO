#include "piemodel.h"

bool PieChartModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    return source_column == 0 && !source_parent.isValid();
}

PieModel::PieModel(View *v, QObject *parent) : Model(v, parent), chart(new PieChart()){}

void PieModel::changeChartTitle(const QString &new_title){
    chart->changeTitle(new_title);
}

void PieModel::getChartFromDocument(const QJsonDocument &doc){ /* DA IMPLEMENTARE */
    chart->getFromJSON(doc);
}

void PieModel::save(const QString &path) const{ /* DA IMPLEMENTARE */

}

int PieModel::rowCount(const QModelIndex &parent) const{
    return (!parent.isValid()) ? chart->slicesCount() : 0;
}

int PieModel::columnCount(const QModelIndex &parent) const{
    return (!parent.isValid()) ? 2 : 0;
}

bool PieModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent, row, row+count-1);
    bool result = (!parent.isValid()) ? chart->insertSlices(row, count) : false;
    endInsertRows();
    return result;
}

bool PieModel::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row+count-1);
    bool result = (!parent.isValid()) ? chart->removeSlices(row, count) : false;
    endRemoveRows();
    return result;
}

QVariant PieModel::data(const QModelIndex &index, int role) const{
    const Slice *current = (!index.parent().isValid()) ? chart->getSlice(index.row()) : nullptr;
    if(current && role == Qt::DisplayRole){
        if(index.column() == 0)
            return QVariant(current->getName());
        if(index.column() ==1);
            return QVariant(current->getValue());
    }
    return QVariant();
}

bool PieModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    const Slice *current = (!index.parent().isValid()) ? chart->getSlice(index.row()) : nullptr;
     if(current){
         if(index.column() == 0 && chart->checkSliceName(value.toString())){
             const_cast<Slice*>(current)->changeName(value.toString());
             emit dataChanged(index, index);
             return true;
         }
         if(index.column() == 1 ){
             const_cast<Slice*>(current)->changeValue(value.toDouble());
             emit dataChanged(index, index);
             return true;
         }
     }
     return false;
}

QModelIndex PieModel::parent(const QModelIndex &index) const{
    return QModelIndex();
}

QModelIndex PieModel::index(int row, int column, const QModelIndex &parent) const{
   const Slice *current = (!parent.isValid()) ? chart->getSlice(row) : nullptr;
   if(current){
       if(column == 0)
           return createIndex(row, column, const_cast<QString*>(&(current->getName())));
       if(column == 1)
           return createIndex(row, column, const_cast<double*>(&(current->getValue())));
   }
   return QModelIndex();
}

