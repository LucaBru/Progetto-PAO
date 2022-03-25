#ifndef LINECHARTWIDGET_H
#define LINECHARTWIDGET_H
#include "View/CentralWidget/ChartWidget/chartwidget.h"
#include <QValueAxis>
#include <vector>
using std::vector;

class LineChartWidget : public ChartWidget{
    Q_OBJECT
private:
    void connectLineModelSignals() const;
    void connectSignals() const;
    void configChartWidgetItems() const;
    void configLineChartWidgetItems() const;
    void configInitialQLineSeries();
    void configInitialQPointFsInQLineSeries(const QModelIndex& line_index);
protected:
    QLineEdit *line_name;
    QList<QLineSeries*> lines;
    QGroupBox *point_info;
    QFormLayout *point_info_layout;
    QComboBox *points;
    QLineEdit *point_x_value;
    QLineEdit *point_y_value;
    QPushButton *insert_point;
    QPushButton *remove_point;
    QModelIndex current_line_index;
public:
    LineChartWidget(View *v, Model *m, QWidget *parent =nullptr);
    void createChartFromModel() override;
protected slots:
    void currentLine(int row);
    void userInsertLine();
    void userRemoveLine();
    void userChangeLineName();
    void currentPoint(int row);
    void userInsertPoint();
    void userRemovePoint();
    void userChangeXPointValue();
    void userChangeYPointValue();
    void lineAtChangedName(int line_row, const QString& new_name);
    void multipleLinesInserted(int row, int count);
    void multipleLinesRemoved(int row, int count);
    void multiplePointsAtLineInserted(int line_row, int point_row, int count);
    void multiplePointsAtLineRemoved(int line_row, int point_row, int count);
    void pointAtLineChanged(int line_row, int row, double new_x, double new_y);
};


#endif // LINECHARTWIDGET_H
