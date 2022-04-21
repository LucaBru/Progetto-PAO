#ifndef BARMODEL_H
#define BARMODEL_H
#include "Model/model.h"
#include "Model/Chart/BarChart/barchart.h"
#include <vector>
using std::vector;

class BarModel : public Model{
    Q_OBJECT
private:
    QVariant getSetName(const QModelIndex& index) const;
    QVariant getSetValue(const QModelIndex& index) const;
    QVariant getColor(const QModelIndex& index) const;
    bool setSetName(const QModelIndex& index, const QString& new_name);
    bool setSetValue(const QModelIndex& index, double new_value);
    bool setSetColor(const QModelIndex& index, const QColor& color);

public:
    BarModel(View *v, const QJsonObject& obj = QJsonObject(), QObject *parent =nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool insertColumns(int column, int count, const QModelIndex &parent) override;
    bool removeColumns(int column, int count, const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    bool changeCategoryNameAt(int index, const QString& new_name);
    bool isCategoryNameValid(const QString& name) const;
    QString getCategoryNameAt(int index) const;

signals:
    void multipleSetsInserted(int row, int count);
    void multipleCategoriesInserted(int column, int count);
    void multipleSetsRemoved(int row, int count);
    void multipleCategoriesRemoved(int column, int count);
    void setAtChangedName(int index, const QString& new_name);
    void setAtChangedValue(int set_index, int value_index, double new_value);
    void categoryAtChangedName(int index, const QString& new_name);

};

#endif // BARMODEL_H
