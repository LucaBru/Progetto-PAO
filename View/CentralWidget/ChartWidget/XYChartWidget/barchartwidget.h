#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H
#include "View/CentralWidget/ChartWidget/XYChartWidget/xychartwidget.h"
#include <QToolButton>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QList>
#include <vector>
#include <QFrame>
using std::vector;

class BarChartWidget;

class CategoryWidget : public QWidget{
    Q_OBJECT
private:
    BarChartWidget *parent;
    QToolButton *garbage;
    QLineEdit *category_name;
    QLineEdit *set_value;
    QToolButton *add_new_category;
    QLineEdit *value_axis_label;

    void confWidgetItems();
    void connectSignalsAndSlots() const;
public:
    unsigned int index;
    CategoryWidget(BarChartWidget *parent, int index);
    void setValue(double val);
    void setCategoryName(const QString& name);
    void setBorderForTextError();
    void setDefaultBorder();
private slots:
    void userRemoveCategory();
    void userAddCategory();
    void userChangeCategoryName();
    void userChangeSetValue();
};

class BarChartWidget : public XYChartWidget{
    Q_OBJECT
private:
    QBarSeries *bar_serie;
    QBarCategoryAxis *categories_axis;
    QLineEdit *set_name;
    QPushButton *add_new_category;
    QFormLayout *cat_items_layout;


    void connectSignalsAndSlots() const;
    void connectBarModelSignals() const;
    void configChartWidgetItems();
    void configBarChartWidgetItems();
    void updateCategoryWidgetItems(int index, int count);
    void updateIndexOfCategoryWidgetItems(int index, int count);
    void updateSetValueOnSeriesCurrentIndexChanged(int index);
    void inizializeNewCategorySetValue(int column, int count);
    void removeSetsValueAtIndex(int index);
    void getSetsFromModel();
    void getCategoriesFromModel();
    void changeSetColor(int set_index);
    void resetCategoryBorderStyle();
    void updateValueAxisRangeOnRemovedSet(QBarSet* set);
    double findNewValueAxisMaxWithOutSet(QBarSet* set =nullptr) const;

public:
    BarChartWidget(View *v, Model *m, QWidget *parent =nullptr);
    void createChartFromModel() override;
    void removeCategory(int index);
    void addCategory(int index);
    void changeCategoryName(int index, const QString& new_name);
    void changeSetValue(int index, double new_value);

private slots:
    void userInsertSet();
    void userRemoveSet();
    void userInsertFirstCategory();
    void userChangeSetName();
    void multipleSetsInserted(int row, int count);
    void multipleCategoriesInserted(int column, int count);
    void multipleSetsRemoved(int row, int count);
    void multipleCategoriesRemoved(int column, int count);
    void setAtChangedName(int index, const QString& new_name);
    void setAtChangedValue(int set_index, int value_index, double new_value);
    void categoryAtChangedName(int index, const QString& new_name);
    void currentSetChanged(int index);
    void barClicked(int index, QBarSet* set);
    void changeColor();
};


/*
 * 2) gestione profonda attributi BarChartWidget
 *
 *
 */

#endif // BARCHARTWIDGET_H
