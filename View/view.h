#ifndef VIEW_H
#define VIEW_H
#include <QObject>
#include <QMainWindow>
#include <QToolBar>

class View : public QObject{
    Q_OBJECT
private:
    void manageOldChart() const;
    void connectSignalsToSlot(QAction *open_from_file, QAction *new_pie_chart, QAction *new_line_chart, QAction *new_bar_chart);
    
    void connectToolBarSignalsToSlots(QAction *new_pie_chart, QAction *new_bar_chart, QAction *new_line_chart, QAction *open_from_file);
    
protected:
    QMainWindow *mainW;
    QToolBar *tool_bar;
public:
    View(QObject *parent =nullptr);
    ~View();

    void setCentralWidget(QWidget* widget);
    void show() const;
    QMainWindow* getMainWindow() const;
    QAction* addActionToToolBar(const QIcon& icon, const QString& text);
    void removeAction(QAction* action);

private slots:
    void openFromFile();
    void newPieChart();
    void newLineChart();
    void newBarChart();
};

#endif // VIEW_H
