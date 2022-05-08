#ifndef VIEW_H
#define VIEW_H
#include <QObject>
#include <QMainWindow>
#include <QToolBar>

class View : public QObject{
    Q_OBJECT
private:
    bool manageOldChart() const;
    void connectToolBarSignalsToSlots(QAction *new_pie_chart, QAction *new_bar_chart, QAction *new_line_chart, QAction *open_from_file) const;
    void configToolBarItems() const;
    QWidget* initialCentralWidget() const;
    
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
