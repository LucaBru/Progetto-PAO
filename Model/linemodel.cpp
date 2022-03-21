#include "linemodel.h"
#include "Model/Chart/LineChart/linechart.h"

LineModel::LineModel(View *v, const QJsonObject& obj, QObject *parent) : Model(v, ((obj.isEmpty()) ? new LineChart() : new LineChart(obj)), parent){}

int LineModel::rowCount(const QModelIndex &parent) const{
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c){
        Line *current_line = (parent.isValid()) ? c->getLine(parent.row()) : nullptr;
        return (current_line) ? current_line->getPointsCount() : c->getLinesCount();
    }
    return 0;
}

int LineModel::columnCount(const QModelIndex &parent) const{
    return (parent.isValid()) ? 3 : 1;
}

bool LineModel::insertRows(int row, int count, const QModelIndex &parent){
    bool result = false;
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c){
        beginInsertRows(parent, row, row+count-1);
        if(!parent.isValid()){
            result = c->insertLines(row, count);
            if(result)
                emit insertMultipleLines(row, count);
        }
        else{
            Line *current_line = c->getLine(parent.row());
            result = (current_line) ? current_line->insertPoints(row, count) : false;
            if(result)
                emit insertMultiplePointsAtLine(parent.row(), row, count);
        }
        endInsertRows();
    }
    return result;
}

bool LineModel::removeRows(int row, int count, const QModelIndex &parent){
    bool result = false;
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c){
        beginRemoveRows(parent, row, row+count-1);
        if(!parent.isValid()){
            result = c->removeLines(row, count);
            if(result)
                emit removeMultipleLines(row, count);
        }
        else{
            Line *current_line = c->getLine(parent.row());
            result = (current_line) ? current_line->removePoints(row, count) : false;
            if(result)
                emit removeMultiplePointsAtLine(parent.row(), row, count);
        }
        endRemoveRows();
    }
    return result;
}

QVariant LineModel::data(const QModelIndex &index, int role) const{
    QVariant result;
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c && role == Qt::DisplayRole){
        if(!index.parent().isValid()){
            if(c->getLine(index.row()))
                result = c->getLine(index.row())->getName();
        }
        else{
            Line *current_line = c->getLine(index.parent().row());
            if(current_line && current_line->getPoint(index.row())){
                Point *current_point = current_line->getPoint(index.row());
                if(index.column() == 0)
                    result = *const_cast<double*>(current_point->getX());
                else if(index.column() == 1)
                    result = *const_cast<double*>(current_point->getY());
                else
                    result = current_point->toQString();
            }
        }
    }
    return result;
}

bool LineModel::setData(const QModelIndex &index, const QVariant &value, int role){
    bool result = false;
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c){
        if(!index.parent().isValid()){
            Line *current_line = c->getLine(index.row());
            if(current_line){
                current_line->changeName(value.toString());
                result = true;
                emit dataChanged(index, index);
                emit lineAtChangedName(index.row(), value.toString());
            }
        }
        else{
            Point *current_point = (c->getLine(index.parent().row())) ? c->getLine(index.parent().row())->getPoint(index.row()) : nullptr;
            if(current_point){
                if(index.column() == 0){
                    current_point->setX(value.toDouble());
                    result = true;
                    emit dataChanged(index, index);
                    emit pointAtLineChanged(index.parent().row(), index.row(), value.toDouble(), *current_point->getY());
                }
                else if(index.column() == 1){
                    current_point->setY(value.toDouble());
                    result = true;
                    emit dataChanged(index, index);
                    emit pointAtLineChanged(index.parent().row(), index.row(), *current_point->getX(), value.toDouble());
                }
            }
        }
    }
    return result;
}

/*
 * grosso problema:
 *  - data potrebbe anche essere di tipo double* (e non solo ChartData*)
 *  Possibile soluzione:
 *  - unica soluzione che mi viene in mente è utilizzare Point come ultimo elemento della gerarchia di dati (Point è un ChartData), set Data e Data ritorneranno un QVariant costruito passando un QPointF come paremetro (costruttore di variant esistente). Il modello assume così un struttura ad albero (composta da righe di una colonna in diverse gerarchie)
 * Incognite: come visualizza points un QPointF? (teoricamnente con una stringa vuota) <- trovare una soluzione a questo problema
 * Possibile soluzione: se è un point_x oppure point_y allora ha per forza un fratello => riesco a trovare il tipo
 * Altrimenti posso lavorare a livello di point
 *
 */

QModelIndex LineModel::parent(const QModelIndex &child) const{
    if(child.isValid()){
        Point *point = dynamic_cast<Point*>(static_cast<ChartData*>(child.internalPointer()));
        LineChart *c = dynamic_cast<LineChart*>(chart);
        if(c && point){
            Line * parent_line = (dynamic_cast<Line*>(point->parentItem()));
            if(parent_line){
                int row = c->getLineIndex(parent_line);
                if(row != -1)
                    return createIndex(row, 0, parent_line);
            }
        }
    }
    return QModelIndex();
}

QModelIndex LineModel::index(int row, int column, const QModelIndex &parent) const{
    QModelIndex i;
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c){
        if(!parent.isValid() && column == 0){
            Line *current_line = c->getLine(row);
            if(current_line)
                i = createIndex(row, column, current_line);
        }
        else{
            Line *current_line = c->getLine(parent.row());
            if(current_line){
                Point *current_point = current_line->getPoint(row);
                i = createIndex(row, column, current_point);
            }
        }
    }
    return i;
}
