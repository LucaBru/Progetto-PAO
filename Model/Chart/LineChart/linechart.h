#ifndef LINECHART_H
#define LINECHART_H
#include "Model/Chart/chart.h"
#include <vector>
using std::vector;

class Line;

class Point : public ChartData{
private:
    Line *parent;
    double x, y;
    bool valid;
public:
    Point(Line* parent, double x =0, double y =0);
    Point(Line* parent, const Point&);
    //costruttore di copia raddoppia la linea creando cosi una linea con il doppio degli elementi, tutti ripetuti 2 volte
    const double* getX() const;
    const double* getY() const;
    bool isValid() const;
    void setValid();
    void setInvalid();
    void setX(double new_x);
    void setY(double new_y);
    QString toQString() const;
    ChartData* parentItem() const override;
    //distruttore NON profondo
};

class Line : public ChartData{
private:
    QString name;
    vector<Point*> points;

    static vector<Point*> copyPoints(const Line& line, Line* to_line);
    static void destroyPoints(Line& line);
    static bool isPointsInLineValid(const QJsonObject& line);
    bool isPointNewXValueCorrect(int point_index, double new_x_value) const;
    Point* findFirstValidPointBefore(int point_index) const;
    Point* findFirstValidPointAfter(int point_index) const;
    void confPointsFromQJsonObject(const QJsonObject& line);
public:
    Line(const QString& name = QString());
    Line(const QJsonObject& line);

    //gestione della memoria profonda
    Line(const Line& line);
    Line& operator =(const Line& line);
    ~Line();

    const QString& getName() const;
    void changeName(const QString& new_name);
    Point* getPoint(int index) const;
    void changeXPointValue(int point_index, double new_x_value);
    bool insertPoints(int index, int count, double x =0, double y =0);
    bool removePoints(int index, int count);
    int getPointsCount() const;
};

class LineChart : public Chart{
private:
    //inserisce la linea solamnete se il nome è valido (ovvero se non ci sono altre linee con tale nome)
    void confLinesFromQJsonObject(const QJsonArray& lines_from_json);
    bool existLineName(const QString& line_name) const;
protected:
    vector<Line*> lines;

    static vector<Line*> copyLines(const LineChart& chart);
    static void destroyLines(LineChart& chart);
public:
    LineChart(const QString& t =QString());
    LineChart(const QJsonObject& obj);

    //gestione della memoria profonda
    LineChart(const LineChart& chart);
    LineChart& operator=(const LineChart& chart);
    ~LineChart();

    bool changeLineName(int line_index, const QString& new_name);
    Line* getLine(int index) const;
    //contratto getLineIndex(const Line* line) ritorna l'indice di line all'interno di lines se esiste, -1 altrimenti
    int getLineIndex(const Line* line) const;
    bool insertLines(int index, int count);
    bool removeLines(int index, int count);
    int getLinesCount() const;
    QJsonObject* parsing() const override;


};

#endif // LINECHART_H
