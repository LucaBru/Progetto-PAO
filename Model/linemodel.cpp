#include "linemodel.h"
#include "Model/Chart/LineChart/linechart.h"

bool LineModel::changeLineName(const QModelIndex &line_index, const QString &new_line_name){
    bool result = false;
    LineChart *c = dynamic_cast<LineChart*>(chart);
    Line *line = dynamic_cast<Line*>(static_cast<ChartData*>(line_index.internalPointer()));
    if(c && line){
        result = c->changeLineName(line_index.row(), new_line_name);
        if(result){
            emit dataChanged(line_index, line_index);
            emit lineAtChangedName(line_index.row(), new_line_name);
        }

    }
    return result;
}

bool LineModel::changeXPointValue(const QModelIndex &point_index, double new_x_value){
    bool result = false;
    Point *point = dynamic_cast<Point*>(static_cast<ChartData*>(point_index.internalPointer()));
    if(point){
        Line *parent = static_cast<Line*>(point->parentItem());
        parent->changeXPointValue(point_index.row(), new_x_value);
        if(point->isValid()){
            result = true;
            emit pointAtLineChanged(point_index.parent().row(), point_index.row(), new_x_value, *point->getY());
        }
        emit dataChanged(point_index, point_index);

    }
    return result;
}

bool LineModel::changeYPointValue(const QModelIndex &point_index, double new_y_value){
    bool result = false;
    Point *point = dynamic_cast<Point*>(static_cast<ChartData*>(point_index.internalPointer()));
    if(point && point->isValid()){
        point->setY(new_y_value);
        if(point->isValid()){
            result = true;
            emit pointAtLineChanged(point_index.parent().row(), point_index.row(), *point->getX(), new_y_value);
        }
    }
    return result;

}

bool LineModel::changePointValue(const QModelIndex &index, double value){
    bool result = false;
    if(index.isValid()){
        if(index.column() == 0)
            result = changeXPointValue(index, value);
        else if(index.column() == 1)
            result = changeYPointValue(index, value);
    }
    return result;
}

QVariant LineModel::getPointValue(const QModelIndex &index) const{
    QVariant result;
    Point *point = dynamic_cast<Point*>(static_cast<ChartData*>(index.internalPointer()));
    if(point){
        if(index.column() == 0)
            result = (point->getX()) ? *const_cast<double*>(point->getX()) : QVariant();
        else if(index.column() == 1)
            result = (point->getY()) ? *const_cast<double*>(point->getY()) : QVariant();
        else
            result = point->toQString();
    }
    return result;
}

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
    if(role == Qt::DisplayRole){
        Line *line = dynamic_cast<Line*>(static_cast<ChartData*>(index.internalPointer()));
        if(line)
            result = line->getName();
        else
            result = getPointValue(index);
    }
    return result;
}

bool LineModel::setData(const QModelIndex &index, const QVariant &value, int role){
    bool result = false;
    if(index.isValid())
        if(!index.parent().isValid())
            result = changeLineName(index, value.toString());
        else
            result = changePointValue(index, value.toDouble());
    return result;
}

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
    Line *line = dynamic_cast<Line*>(static_cast<ChartData*>(parent.internalPointer()));
    LineChart *c = dynamic_cast<LineChart*>(chart);
    if(c){
        if(!line && column == 0 && c->getLine(row))
            i = createIndex(row, column, c->getLine(row));
        else if(line && line->getPoint(row))
            i = createIndex(row, column, line->getPoint(row));
    }
    return i;
}
