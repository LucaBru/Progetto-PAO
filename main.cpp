#include <QApplication>
#include "View/view.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    View *view = new View();
    view->show();
    return a.exec();
}
/*
 * Line Model problema assi da risolvere
 */

