#include "piemodel.h"

bool PieModel::changeSliceName(int index, const QString &new_name){
    bool result = static_cast<PieChart*>(chart)->changeSliceAtName(index, new_name);
    if(result)
        emit sliceAtNameChanged(index, new_name);
    return result;
}

bool PieModel::changeSliceValue(int index, double new_value){
    bool result = false;
    Slice *slice = static_cast<PieChart*>(chart)->getSlice(index);
    if(slice){
        slice->changeValue(new_value);
        emit sliceAtValueChanged(index, new_value);
        result = true;
    }
    return result;
}

bool PieModel::changeSliceColor(int index, const QColor &new_color){
    bool result = false;
    Slice *slice = static_cast<PieChart*>(chart)->getSlice(index);
    if(slice){
        slice->changeColor(new_color);
        result = true;
    }
    return result;
}

PieModel::PieModel(View *v, const QJsonObject& obj, QObject *parent) : Model(v, ((obj.isEmpty()) ? new PieChart() : new PieChart(obj)), parent){}

int PieModel::rowCount(const QModelIndex &parent) const{
    return static_cast<PieChart*>(chart)->slicesCount();
}

int PieModel::columnCount(const QModelIndex &parent) const{
    return (!parent.isValid()) ? 3 : 0;
}

bool PieModel::insertRows(int row, int count, const QModelIndex &parent){
    beginInsertRows(parent, row, row+count-1);
    bool result = static_cast<PieChart*>(chart)->insertSlices(row, count);
    endInsertRows();
    if(result)
        emit addMultipleSlices(row, count);
    return result;
}

bool PieModel::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row+count-1);
    bool result = static_cast<PieChart*>(chart)->removeSlices(row, count);
    endRemoveRows();
    if(result)
        emit removeMultipleSlices(row, count);
    return result;
}

QVariant PieModel::data(const QModelIndex &index, int role) const{
    QVariant result;
    const Slice *current = static_cast<PieChart*>(chart)->getSlice(index.row());
    int column = index.column();
    if(current && role == Qt::DisplayRole){
        if(column == 0)
            result = current->getName();
        else if(column == 1)
            result = current->getValue();
    }
    else if(current && role == Qt::DecorationRole && column == 0)
        result = current->getColor();
    return result;
}

bool PieModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool result = false;
    int column = index.column();
    int row = index.row();
    if(role == Qt::EditRole){
        if(column == 0)
            result = changeSliceName(row, value.toString());
        else if(column == 1)
            result = changeSliceValue(row, value.toDouble());

        if(result)
            emit dataChanged(index, index);
     }
     else if(role == Qt::DecorationRole && column == 0)
         result = changeSliceColor(index.row(), value.value<QColor>());
     return result;
}

QModelIndex PieModel::parent(const QModelIndex &index) const{
    return QModelIndex();
}

QModelIndex PieModel::index(int row, int column, const QModelIndex &parent) const{
   Slice *slice = static_cast<PieChart*>(chart)->getSlice(row);
   QModelIndex index;
   if(slice)
        index = createIndex(row, column, slice);
   return index;
}

