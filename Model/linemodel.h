#ifndef LINEMODEL_H
#define LINEMODEL_H
#include "Model/model.h"

class LineModel : public Model{
    Q_OBJECT
public:
    LineModel(View *v, const QJsonObject& obj = QJsonObject(), QObject *parent =nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
signals:
    void insertMultipleLines(int row, int count);
    void removeMultipleLines(int row, int count);
    void lineAtChangedName(int row, const QString& name);
    void pointAtLineChanged(int line_index, int point, double new_x, double new_y);
    void insertMultiplePointsAtLine(int line_row, int row, int count);
    void removeMultiplePointsAtLine(int line_row, int row, int count);
};

#endif // LINEMODEL_H
