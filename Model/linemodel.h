#ifndef LINEMODEL_H
#define LINEMODEL_H
#include "Model/model.h"

class LineModel : public Model{
    Q_OBJECT
private:
    bool changeLineName(const QModelIndex& line_index, const QString& new_line_name);
    //pre condizioni: chart è LineChart e index si riferisce ad un Point => index.parent.isValid() == true
    bool changeXPointValue(const QModelIndex& point_index, double new_x_value);
    bool changeYPointValue(const QModelIndex& point_index, double new_y_value);
    bool changePointValue(const QModelIndex& index, double value);
    QVariant getPointValue(const QModelIndex& index) const;
    bool insertLines(int row, int count);
    bool insertPointsInLine(int row, int count, const QModelIndex& line_parent);
    bool removeLines(int row, int count);
    bool removePointsInLine(int row, int count, const QModelIndex& line_parent);
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
