#ifndef PIECHART_H
#define PIECHART_H
#include <QColor>
#include "Model/Chart/chart.h"
#include <cstdlib>
#include <vector>
using std::vector;

class Slice : public ChartData{
private:
    QString name;
    double value;
    QColor color;
public:
    Slice(const QString& n = QString(), double v = 0, const QColor& color =QColor(rand()%256, rand()%256, rand()%256));
    Slice(const QJsonObject& obj);
    QString getName() const;
    double getValue() const;
    QColor getColor() const;
    void changeName(const QString& newName);
    void changeValue(double newValue);
    void changeColor(const QColor& new_color);
};

class PieChart : public Chart{
private:
    vector<Slice*> slices; //invariante: Slice->name è univoco per ogni Slice in slices (sse name != QString())

    static vector<Slice*> copySlices(const PieChart& from);
    static void deleteSlices(PieChart& pieChart);
    bool isSliceNameValid(const QString& name) const;
public:
    PieChart(const QString& t = QString());
    PieChart(const QJsonObject& obj);

    //gestione profonda della memoria
    PieChart(const PieChart& from);
    PieChart& operator=(const PieChart& from);
    ~PieChart();
    bool insertSlices(int row, int count);
    bool removeSlices(int row, int count);
    Slice* getSlice(int index) const;
    int slicesCount() const;

    bool changeSliceAtName(int index, const QString& new_name);
    QJsonObject* parsing() const override;
};

#endif // PIECHART_H

