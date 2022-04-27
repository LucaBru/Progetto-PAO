#include <QApplication>
#include "View/view.h"
#include "QtCharts/QtCharts"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    View *view = new View();
    view->show();
    return a.exec();
}

/*
 * manca qualche piccolezza, rivedere bordo di QLineEdit per il resto ok
 *
 */
