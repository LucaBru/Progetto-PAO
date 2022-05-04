#ifndef BARCHART_H
#define BARCHART_H
#include "Model/Chart/XYChart/xychart.h"
#include <QColor>
#include <vector>
using std::vector;

class Set : public ChartData{
protected:
    QString name;
    QColor color;
    vector<double> values; //invariante, tanti quanti le categorie
public:
    Set(int values_size);
    Set(const QString& name, vector<double> &arr);
    Set(const QJsonObject& obj);
    QString getName() const;
    int getValuesSize() const;
    double getValueOfCategoryAt(int cat_index);
    QColor getColor() const;
    bool changeValueOfCategoryAt(int cat_index, double new_value);

    void changeColor(const QColor& color);
    void changeName(const QString& new_name);
    void insertValue(int index, double value);
    bool removeValuesAt(int index, int count);
    QJsonObject parsing() const;
};

class BarChart : public XYChart{
private:
    static vector<Set*> copySets(const BarChart& from);
    static void destroySets(BarChart& from);
    bool isSetNameValid(const QString& name) const;
    bool insertNewCategoryValues(int index);
    bool removeAllCategoryValues(int cat_index);
    void getSetsFromJson(const QJsonArray& jsets);
    void getCategoriesNameJson(const QJsonArray& jcategories);
    static bool isBarChartConsistent(const QJsonObject& obj);

protected:
    //invariante da mantenere: ogni set in sets deve avere un array di valori di dimensione = categories.size()
    vector<QString> categories;
    vector<Set*> sets;
public:
    BarChart(const QString& title = QString());
    BarChart(const QJsonObject& obj);

    //gestione della memoria profonda
    BarChart(const BarChart& chart);
    BarChart& operator =(const BarChart& chart);
    ~BarChart();

    bool insertCategories(int index, int count);
    bool insertSetsAt(int index, int count);
    bool changeSetNameAt(int set_index, const QString& new_name);
    bool changeCategoryNameAt(int cat_index, const QString& new_name);
    bool removeCategoryAt(int cat_index);
    bool removeSetsAt(int set_index, int count);
    bool isCategoryNameValid(const QString& name) const;
    Set* getSetAt(int index);
    QString getCategoryNameAt(int index) const;

    int setsSize() const;
    int categoriesSize() const;

    QJsonObject* parsing() const override;
};


/*
 * Metodi disponibili in BarChart:
 *  - inserimento di una nuova categoria, imposto i valori dei set a 0 se l'array passato non ha la dimensione corretta -> appendCategory()
 *  - inserimento di un nuovo set, imposto tutti i valori iniziali a 0;
 *  - modifica del nome di un set -> changeSetNameAt(index, QString)
 *  - modifica del nome di una categoria -> changeCategoryNameAt(index, QString)
 *  - modifica del valore di un set di una specifica categoria -> getSetAt ->changeValueOfCategoryAt
 *  - rimozione di una categoria -> removeCategoryAt(index)
 *  - rimozione di un set -> removeSet(index)
 *
 */

#endif // BARCHART_H
