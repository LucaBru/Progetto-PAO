#include "chart.h"

Chart::Chart(const QString &t) : title(t){}

void Chart::changeTitle(const QString &t){
    title = t;
}

