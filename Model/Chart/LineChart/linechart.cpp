#include "linechart.h"
#include <QJsonArray>

// --------------------- Point -------------------------

Point::Point(Line* p, double x_x, double y_y) : parent(p), x(x_x), y(y_y){}

Point::Point(Line *p, const Point& point) : parent(p), x(point.x), y(point.y){}

ChartData* Point::parentItem() const{
    return parent;
}

const double* Point::getX() const{
    return &x;
}

const double* Point::getY() const{
    return &y;
}

void Point::setX(double new_x){
    x = new_x;
}

void Point::setY(double new_y){
    y = new_y;
}

QString Point::toQString() const{
    return QString("x: " + QString::number(x) + " y: " + QString::number(y));
}

// --------------------- Line --------------------------

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

Line::Line(const QString &n) : name(n){}

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

Point* Line::getPoint(int index) const{
    return (index >= 0 && index < points.size()) ? points[index] : nullptr;
}

void Line::changeName(const QString &n){
    name = n;
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

LineChart::LineChart(const QJsonObject &obj){}

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

/*
 * Parsing di un chart con 2 linee con 2 punti ciascuna
 * {
 *  title : <chart_title>
 *  type : "line"
 *  lines [
 *     {
 *      line name : <line name>
 *      x values [ <1.x>, <2.x> ]
 *      y values [ <1.y>, <2.y> ]
       },
       {
        line name : <line name>
        x values [<1.x>, <2.y> ]
        y values [ <1.y>, <2.y> ]
       }
 *   ]
 *  }
 *
 */





