#include "linechart.h"
#include <QJsonArray>

// --------------------- Point -------------------------

Point::Point(Line* p, double x_x, double y_y) : parent(p), x(x_x), y(y_y), valid(false){}

Point::Point(Line *p, const Point& point) : parent(p), x(point.x), y(point.y), valid(false){}

ChartData* Point::parentItem() const{
    return parent;
}

double Point::getX() const{
    return x;
}

double Point::getY() const{
    return y;
}

bool Point::isValid() const{
    return valid;
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

Point* Line::findFirstValidPointBefore(int point_index) const{
    Point *result = nullptr;
    if(point_index > 0 && point_index < points.size()){
        for(int i = point_index-1; i >= 0 && !result ; --i)
            result = (points[i]->isValid()) ? points[i] : nullptr;
    }
    return result;
}

Point* Line::findFirstValidPointAfter(int point_index) const{
    Point * result = nullptr;
    if(point_index >= 0 && point_index < points.size()-1){
        for(int i = point_index+1; i < points.size() && !result; ++i)
            result = points[i]->isValid() ? points[i] : nullptr;
    }
    return result;
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

Line::Line(const QString &n, const QColor& c) : name(n), color(c){}

Line::Line(const QJsonObject& line){
    if(!line.isEmpty()){
        name = line.value("name").toString();
        color = line.value("color").toString();
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

QColor Line::getColor() const{
    return color;
}

void Line::changeColor(const QColor &new_color){
    color = new_color;
}

bool Line::isPointNewXValueCorrect(int point_index, double new_x_value) const{
    Point *before = findFirstValidPointBefore(point_index);
    bool b = (before) ? before->getX() < new_x_value : true;
    Point *after = findFirstValidPointAfter(point_index);
    bool a = (after) ? after->getX() > new_x_value : true;
    return b && a;
}

void Line::changeXPointValue(int point_index, double new_x_value){
    if(isPointNewXValueCorrect(point_index, new_x_value)){
        points[point_index]->setX(new_x_value);
        points[point_index]->setValid();
    }
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

QJsonArray Line::getXValuesAsJsonArray() const{
    QJsonArray x_points;
    for(vector<Point*>::const_iterator i = points.begin(); i != points.end(); ++i)
        x_points.append((*i)->getX());
    return x_points;
}

QJsonArray Line::getYValuesAsJsonArray() const{
    QJsonArray y_points;
    for(vector<Point*>::const_iterator i = points.begin(); i != points.end(); ++i)
        y_points.append((*i)->getY());
    return y_points;
}

// --------------------- LineChart -----------------------

void LineChart::confLinesFromQJsonObject(const QJsonArray& lines_from_json){
    for(QJsonArray::const_iterator i = lines_from_json.constBegin(); i != lines_from_json.constEnd(); ++i){
        if(!existLineName(i->toObject().value("name").toString()))
            lines.push_back(new Line(i->toObject()));
    }

}

bool LineChart::existLineName(const QString &line_name) const{
    bool exist = false;
    for(vector<Line*>::const_iterator i = lines.begin(); i != lines.end() && !exist; ++i)
        exist = (*i)->getName().compare(line_name, Qt::CaseInsensitive) == 0;
    return exist;
}

QJsonArray LineChart::getLinesAsJsonArray() const{
    QJsonArray lines_in_chart;
    for(vector<Line*>::const_iterator i = lines.begin(); i != lines.end(); ++i){
        QJsonObject line;
        line.insert("name", (*i)->getName());
        line.insert("color", (*i)->getColor().name());
        line.insert("x values", (*i)->getXValuesAsJsonArray());
        line.insert("y values", (*i)->getYValuesAsJsonArray());
        lines_in_chart.append(line);
    }
    return lines_in_chart;
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

LineChart::LineChart(const QString &t) : XYChart(t){}

LineChart::LineChart(const QJsonObject &obj) : XYChart(obj){
    confLinesFromQJsonObject(obj.value("lines").toArray());
}

LineChart::LineChart(const LineChart& chart) : XYChart(chart), lines(copyLines(chart)){}

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

bool LineChart::changeLineName(int line_index, const QString &new_name){
    bool result = false;
    if(line_index >= 0 && line_index < lines.size() && !existLineName(new_name)){
        lines[line_index]->changeName(new_name);
        result = true;
    }
    return result;
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
    QJsonObject* obj = XYChart::parsing();
    obj->insert("type", "line");
    obj->insert("lines", getLinesAsJsonArray());
    return obj;
}

int LineChart::getLinesCount() const{
    return lines.size();
}
