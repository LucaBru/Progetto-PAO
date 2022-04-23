#ifndef VIEW_H
#define VIEW_H
#include <QObject>
#include <QMainWindow>
#include <QToolBar>

class View : public QObject{
    Q_OBJECT
private:
    void manageOldChart() const;
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
