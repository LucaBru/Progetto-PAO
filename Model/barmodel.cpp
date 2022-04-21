#include "barmodel.h"

QVariant BarModel::getSetName(const QModelIndex &index) const{
    QVariant result;
    Set *set = dynamic_cast<Set*>(static_cast<ChartData*>(index.internalPointer()));
    if(set)
        result = set->getName();
    return result;
}

QVariant BarModel::getSetValue(const QModelIndex &index) const{
    QVariant result;
    Set *set = dynamic_cast<Set*>(static_cast<ChartData*>(index.internalPointer()));
    if(set)
        result = set->getValueOfCategoryAt(index.column()-1);
    return result;
}

bool BarModel::setSetName(const QModelIndex &index, const QString &new_name){
    bool result = static_cast<BarChart*>(chart)->changeSetNameAt(index.row(), new_name);
    if(result){
        emit dataChanged(index, index);
        emit setAtChangedName(index.row(), new_name);
    }
    return result;
}

bool BarModel::setSetValue(const QModelIndex &index, double new_value){
    bool result = false;
    Set *set = dynamic_cast<Set*>(static_cast<ChartData*>(index.internalPointer()));
    if(set){
        result = set->changeValueOfCategoryAt(index.column()-1, new_value);
        if(result){
            emit dataChanged(index, index);
            emit setAtChangedValue(index.row(), index.column(), new_value);
        }
    }
    return result;
}

BarModel::BarModel(View *v, const QJsonObject &obj, QObject *parent) : Model(v, (obj.isEmpty()) ? new BarChart() : new BarChart(obj), parent){}

int BarModel::rowCount(const QModelIndex &parent) const{
    if(!parent.isValid())
        return static_cast<BarChart*>(chart)->setsSize();
    return 0;
}

int BarModel::columnCount(const QModelIndex &parent) const{
    return 1+static_cast<BarChart*>(chart)->categoriesSize();
}

bool BarModel::insertRows(int row, int count, const QModelIndex &parent){
    bool result = false;
    if(!parent.isValid()){
        beginInsertRows(parent, row, row+count-1);
        result = static_cast<BarChart*>(chart)->insertSetsAt(row, count);
        if(result)
            emit multipleSetsInserted(row, count);
        endInsertRows();
    }
    return result;
}

bool BarModel::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row+count-1);
    bool result = static_cast<BarChart*>(chart)->removeSetsAt(row, count);
    if(result)
        emit multipleSetsRemoved(row, count);
    endRemoveRows();
}

bool BarModel::insertColumns(int column, int count, const QModelIndex &parent){
    //posso voler inserire una categoria dove voglio, nel BarChartView andrò a rimuovere e reinserire con il nuovo ordine le categorie
    bool result = true;
    BarChart* c =  static_cast<BarChart*>(chart);
    column--;
    if(column >= 0){
        beginInsertColumns(parent, column+1, column+count);
            result = c->insertCategories(column, count);
        if(result)
            emit multipleCategoriesInserted(column, count);
        endInsertColumns();
    }
    return result;
}

bool BarModel::removeColumns(int column, int count, const QModelIndex &parent){
    column--;
    if(column >= 0 && column+count-1 < static_cast<BarChart*>(chart)->categoriesSize()){
        beginRemoveColumns(parent, column+1, column+count);
        for(int i = 0; i < count; ++i)
            static_cast<BarChart*>(chart)->removeCategoryAt(column+i);
        emit multipleCategoriesRemoved(column, count);
        endRemoveColumns();
        return true;
    }
    return false;
}

QVariant BarModel::data(const QModelIndex &index, int role) const{
    QVariant result;
    if(role == Qt::DisplayRole){
        if(index.column() == 0)
            result = getSetName(index);
        else{
            result = getSetValue(index);
        }
    }
    return result;
}

bool BarModel::setData(const QModelIndex &index, const QVariant &value, int role){
    bool result = false;
    if(index.column() == 0)
        result = setSetName(index, value.toString());
    else
        result = setSetValue(index, value.toDouble());
    return result;
}

QModelIndex BarModel::parent(const QModelIndex &child) const{
    return QModelIndex();
}

QModelIndex BarModel::index(int row, int column, const QModelIndex &parent) const{
    QModelIndex index;
    if(!parent.isValid()){
        Set *set = static_cast<BarChart*>(chart)->getSetAt(row);
        if(set && column-1 < set->getValuesSize())
            index = createIndex(row, column, set);
    }
    return index;
}

bool BarModel::changeCategoryNameAt(int index, const QString &new_name){
    bool result = static_cast<BarChart*>(chart)->changeCategoryNameAt(index, new_name);
    if(result)
        emit categoryAtChangedName(index, new_name);
    return result;
}

bool BarModel::isCategoryNameValid(const QString &name) const{
    return static_cast<BarChart*>(chart)->isCategoryNameValid(name);
}

QString BarModel::getCategoryNameAt(int index) const{
    return static_cast<BarChart*>(chart)->getCategoryNameAt(index);
}




/*
 * + rootIndex : QModelIndex()
 *  + set : nome (0) + un valore per ogni categoria

 * per le categorie metodi specifici in quanto non sono ChartData
 */
