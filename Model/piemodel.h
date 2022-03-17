#ifndef PIEMODEL_H
#define PIEMODEL_H
#include "Model/model.h"
#include "Model/Chart/PieChart/piechart.h"
#include <QSortFilterProxyModel>

class PieModel : public Model{
    Q_OBJECT
public:
    PieModel(View *v, const QJsonObject& obj = QJsonObject(), QObject * parent =nullptr);
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    //contratto insertRows : true sse inserisce dalla count righe a partire da row (incluso)
    bool insertRows(int row, int count, const QModelIndex& parent) override;
    //contratto removeRows : true sse elimino count righe a partire da row(inclusa)
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QModelIndex parent(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
signals:
    void addMultipleSlices(int row, int count);
    void removeMultipleSlices(int row, int count);
    void sliceAtNameChanged(int row, const QString& name);
    void sliceAtValueChanged(int row, double value);

};

#endif // PIEMODEL_H
