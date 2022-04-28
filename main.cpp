#include <QApplication>
#include "View/view.h"
#include "QtCharts/QtCharts"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("..\\Chart-Application\\icon\\app.ico"));
    View *view = new View();
    view->show();
    return a.exec();
}
