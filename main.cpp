#include <QApplication>
#include "View/view.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    View *view = new View();
    view->show();
    return a.exec();
}

/*
 *
 * rivedere collegamenti su PieChartWidget e verificare la correttezza del modello
 * sistemare QComboBox in maniera tale che si vedano solamnete i nome e non i valori
 *
 *
 */

