#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H
#include <QWidget>
#include "View/view.h"

class CentralWidget : public QWidget{
protected:
    View *view;
public:
    CentralWidget(View *v, QWidget *parent =nullptr);
    //distruttore di QQWidget virtuale => distruttore di CentralWidget e classi derivate virtuale
    //il distruttore di QWidget elimina e distrugge tutti i figli del widget => tutti gli items delle sottoclassi devono essere figli di
    //QWidget => no garbage
};

#endif // CENTRALWIDGET_H
