#include "linechart.h"
#include <QJsonArray>

// --------------------- Point -------------------------

Point::Point(Line* p, double x_x, double y_y) : parent(p), x(x_x), y(y_y), valid(false){}

Point::Point(Line *p, const Point& point) : parent(p), x(point.x), y(point.y), valid(false){}

ChartData* Point::parentItem() const{
    return parent;
}

const double* Point::getX() const{
    return (valid) ? &x : nullptr;
}

const double* Point::getY() const{
    return (valid) ? &y : nullptr;
}

void Point::setValid(){
    valid = true;
}

void Point::setInvalid(){
    valid = false;
}

void Point::setX(double new_x){
    x = new_x;
}

void Point::setY(double new_y){
    y = new_y;
}

QString Point::toQString() const{
    return (valid) ? QString("x: " + QString::number(x) + " y: " + QString::number(y)) : "x: y: ";
}

// --------------------- Line --------------------------

bool Line::isPointsInLineValid(const QJsonObject &line){
    QJsonArray xvalues = line.value("x values").toArray();
    QJsonArray yvalues = line.value("y values").toArray();
    bool is_valid = xvalues.size() == yvalues.size();
    if(is_valid && xvalues.size() >= 2){
        QJsonArray::const_iterator x_iter = xvalues.constBegin()+1;
        QJsonArray::const_iterator x_prec_iter = xvalues.constBegin();
        for(; x_iter != xvalues.constEnd() && is_valid; ++x_iter, ++ x_prec_iter)
            is_valid = (x_prec_iter)->toDouble() < x_iter->toDouble();
    }
    return is_valid;
}

vector<Point*> Line::copyPoints(const Line &line, Line* to_line){
    vector<Point*> result;
    for(vector<Point*>::const_iterator i = line.points.begin(); i != line.points.end(); ++i)
        result.push_back(new Point(to_line, **i));
    return result;
}

void Line::destroyPoints(Line &line){
    for(vector<Point*>::iterator i = line.points.begin(); i != line.points.end(); ++i)
        delete *i;
}

void Line::confPointsFromQJsonObject(const QJsonObject &line){
    //scelta progettuale: se il file viene compromesso e i 2 vettori hanno dimensione diversa oppure le x non sono in ordine strettamente          crescente => non creo nessun punto
    if(isPointsInLineValid(line)){
        QJsonArray xvalues = line.value("x values").toArray();
        QJsonArray yvalues = line.value("y values").toArray();
        QJsonArray::const_iterator x_iter = xvalues.constBegin();
        QJsonArray::const_iterator y_iter = yvalues.constBegin();
        for(; x_iter != xvalues.constEnd(); ++x_iter, ++y_iter){
            Point *new_point = new Point(this, x_iter->toDouble(), y_iter->toDouble());
            new_point->setValid();
            points.push_back(new_point);
        }
    }
}

Line::Line(const QString &n) : name(n){}

Line::Line(const QJsonObject& line){
    if(!line.isEmpty()){
        name = line.value("line name").toString();
        confPointsFromQJsonObject(line);
    }
}

Line::Line(const Line& line) : name(line.name), points(copyPoints(line, this)){}

Line& Line::operator=(const Line& line){
    if(this != &line){
        destroyPoints(*this);
        points = copyPoints(line, this);
    }
    return *this;
}

Line::~Line(){
    destroyPoints(*this);
}

const QString& Line::getName() const{
    return name;
}

void Line::changeName(const QString &new_name){
    name = new_name;
}

Point* Line::getPoint(int index) const{
    return (index >= 0 && index < points.size()) ? points[index] : nullptr;
}

void Line::changePointValue(int point_index, Point *new_point){

}


bool Line::insertPoints(int index, int count, double x, double y){
    if(index >= 0 && count > 0 && index <= points.size()){
        for(int i = 0; i < count; ++i)
            points.insert(points.begin()+index+i, new Point(this));
        return true;
    }
    return false;
}

bool Line::removePoints(int index, int count){
    if(index >= 0 && count > 0 && index+count-1 < points.size()){
        for(int i = 0; i < count; ++i)
            delete points[index+i];
        points.erase(points.begin()+index, points.begin()+index+count);
        return true;
    }
    return false;
}

int Line::getPointsCount() const{
    return points.size();
}

// --------------------- LineChart -----------------------

void LineChart::confLinesFromQJsonObject(const QJsonArray& lines_from_json){
    for(QJsonArray::const_iterator i = lines_from_json.constBegin(); i != lines_from_json.constEnd(); ++i){
        if(!existLineName(i->toObject().value("line name").toString()))
            lines.push_back(new Line(i->toObject()));
    }

}

bool LineChart::existLineName(const QString &line_name) const{
    bool exist = false;
    for(vector<Line*>::const_iterator i = lines.begin(); i != lines.end() && !exist; ++i)
        exist = (*i)->getName().compare(line_name, Qt::CaseInsensitive) == 0;
    return exist;
}

vector<Line*> LineChart::copyLines(const LineChart& chart){
   vector<Line*> result;
   for(vector<Line*>::const_iterator i = chart.lines.begin(); i != chart.lines.end(); ++i)
       result.push_back(new Line(**i));
   return result;
}

void LineChart::destroyLines(LineChart &chart){
    for(vector<Line*>::iterator i = chart.lines.begin() ; i != chart.lines.end(); ++i)
        delete *i;
}

LineChart::LineChart(const QString &t) : Chart(t){}

LineChart::LineChart(const QJsonObject &obj) : Chart(obj){
    confLinesFromQJsonObject(obj.value("lines").toArray());
}

LineChart::LineChart(const LineChart& chart) : Chart(chart), lines(copyLines(chart)){}

LineChart& LineChart::operator =(const LineChart& chart){
    if(this != &chart){
        destroyLines(*this);
        lines = copyLines(chart);
    }
    return *this;
}

LineChart::~LineChart(){
    destroyLines(*this);
}

void LineChart::changeLineName(int line_index, const QString &new_name){
    if(line_index >= 0 && line_index < lines.size() && !existLineName(new_name))
        lines[line_index]->changeName(new_name);

}

Line* LineChart::getLine(int index) const{
    return (index >= 0 && index < lines.size()) ? lines[index] : nullptr;
}

int LineChart::getLineIndex(const Line *line) const{
    int index = -1;
    for(int i = 0; i < lines.size() && index == -1; ++i)
        if(lines[i] == line)
            index = i;
    return index;
}

bool LineChart::insertLines(int index, int count){
    if(index >= 0 && count > 0 && index <= lines.size()){
        for(vector<Line*>::iterator i = lines.begin()+index; count > 0; --count, ++i)
            lines.insert(i, new Line());
        return true;
    }
    return false;
}

bool LineChart::removeLines(int index, int count){
    if(index >= 0 && count > 0 && index+count-1 < lines.size()){
        for(int i = 0; i < count; ++i)
            delete lines[index+i];
        lines.erase(lines.begin()+index, lines.begin()+index+count); //[first, last)
        return true;
    }
    return false;
}

QJsonObject* LineChart::parsing() const{
    QJsonObject* obj = Chart::parsing();
    QJsonArray lines_in_chart;
    for(vector<Line*>::const_iterator i = lines.begin(); i != lines.end(); ++i){
        QJsonObject current_line;
        current_line.insert("line name", (*i)->getName());
        int points_in_line = (*i)->getPointsCount();
        QJsonArray x_values;
        QJsonArray y_values;
        for(int j=0; j < points_in_line; ++j){
            x_values.push_back(*(*i)->getPoint(j)->getX());
            y_values.push_back(*(*i)->getPoint(j)->getY());
        }
        current_line.insert("x values", x_values);
        current_line.insert("y values", y_values);
        lines_in_chart.push_back(current_line);
    }
    obj->insert("type", "line");
    obj->insert("lines", lines_in_chart);
    return obj;
}

int LineChart::getLinesCount() const{
    return lines.size();
}





