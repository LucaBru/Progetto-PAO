#ifndef VIEW_H
#define VIEW_H
#include <QObject>
#include <QMainWindow>

class View : public QObject{
    Q_OBJECT
protected:
    QMainWindow *mainW;
public:
    View(QObject *parent =nullptr);
    ~View();

    void setCentralWidget(QWidget* widget);
    void show() const;
    QMainWindow* getMainWindow() const;

public slots:
    void createFromUserInput();
    void createPieChart();
    void createLineChart();
    void createBarChart();
};

#endif // VIEW_H
