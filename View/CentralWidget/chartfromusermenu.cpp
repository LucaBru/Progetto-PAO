#include "chartfromusermenu.h"
#include <QVBoxLayout>

ChartFromUserMenu::ChartFromUserMenu(View *v, QWidget *parent) : CentralWidget(v, parent), createpiechart(new QPushButton("create pie chart")), createlinechart(new QPushButton("create line chart")), createbarchart(new QPushButton("create bar chart")){

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(createpiechart);
    l->addWidget(createlinechart);
    l->addWidget(createbarchart);

    QObject::connect(createpiechart, SIGNAL(clicked()), view, SLOT(createPieChart()));
    QObject::connect(createlinechart, SIGNAL(clicked()), view, SLOT(createLineChart()));
    QObject::connect(createbarchart, SIGNAL(clicked()), view, SLOT(createBarChart()));
}
