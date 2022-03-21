#ifndef PIECHART_H
#define PIECHART_H
#include "Model/Chart/chart.h"
#include <vector>
using std::vector;

class Slice : public ChartData{
private:
    QString name;
    double value;
public:
    Slice(const QString& n = QString(), double v = 0);
    const QString& getName() const;
    const double& getValue() const;
    void changeName(const QString& newName);
    void changeValue(double newValue);
};

class PieChart : public Chart{
private:
    vector<Slice*> slices; //invariante: Slice->name è univoco per ogni Slice in slices (sse name != QString())

    static vector<Slice*> copySlices(const PieChart& from);
    static void deleteSlices(PieChart& pieChart);
public:
    PieChart(const QString& t = QString());
    PieChart(const QJsonObject& obj);

    //gestione profonda della memoria
    PieChart(const PieChart& from);
    PieChart& operator=(const PieChart& from);
    ~PieChart();
    bool insertSlices(int row, int count);
    bool removeSlices(int row, int count);
    Slice* getSlice(int index) const; //può tornare un nullptr
    int slicesCount() const;
    bool checkSliceName(const QString& name) const;
    QJsonObject* parsing() const override;
};

#endif // PIECHART_H

