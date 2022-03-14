#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H
#include <QWidget>
#include "View/view.h"

class CentralWidget : public QWidget{
protected:
    View *view;
public:
    CentralWidget(View *v, QWidget *parent =nullptr);
};

#endif // CENTRALWIDGET_H
