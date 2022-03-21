#include <QApplication>
#include "View/view.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    View *view = new View();
    view->show();
    return a.exec();
}

/*
 * problema: scelta decisionale
 *  leggere commento in linechartwidget
 *
 *
 *
 *  --- vecchio ----
 *
 * SCELTA FINALE: - TOP -
 *  - i QCOmboBox sono collegati al model => quando si aggiorna il model si aggiorna anche la vista =>
 *  sfrutto segnali di aggiornamento dai QComboBox per aggiornare il chart => ottengo i vantaggi da tutte e due le soluzioni
 *  (in quanto i dati dai chart sono presenti nelle QComboBox in tutti i casi!)
 *  - da sistemare le viste e poi riprendere a programmare
 *  non possibile in quanto non ci sono slot che lo permettono =>
 *
 * Ogni Model emette, in base alle operazioni che fa un segnale specifico (che entra a far parte della documentazione del model stesso), ogni vista collega il segnale con uno slot apposito (la vista in connect dovrà effettuare un cast del model nel modello specifico per ottenere il segnale)
 * Vantaggi di questa soluzione:
 * - a livello computazionale non troppo complicata
 * - sempre coerenza tra i dati nel model e quelli nella vista
 * - segnali specifici che ci permettono di semplificare la vita nella vista
 * - rispetto il pattern (il quale dice che è il modello che deve informare la vista dei cambiamenti sulla base dati)
 */

