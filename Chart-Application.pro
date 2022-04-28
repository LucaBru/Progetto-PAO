QT       += core gui
QT       += widgets
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_ICONS = "..\\Chart-Application\\icon\\app.ico"
ICON = "..\\Chart-Application\\icon\\app.icns" #necessario per icona app su mac. Sufficiente creare questo file
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Model/Chart/BarChart/barchart.cpp \
    Model/Chart/LineChart/linechart.cpp \
    Model/Chart/PieChart/piechart.cpp \
    Model/Chart/chart.cpp \
    Model/barmodel.cpp \
    Model/linemodel.cpp \
    Model/model.cpp \
    Model/piemodel.cpp \
    View/CentralWidget/ChartWidget/barchartwidget.cpp \
    View/CentralWidget/ChartWidget/chartwidget.cpp \
    View/CentralWidget/ChartWidget/linechartwidget.cpp \
    View/CentralWidget/ChartWidget/piechartwidget.cpp \
    View/CentralWidget/centralwidget.cpp \
    View/view.cpp \
    main.cpp

HEADERS += \
    Model/Chart/BarChart/barchart.h \
    Model/Chart/LineChart/linechart.h \
    Model/Chart/PieChart/piechart.h \
    Model/Chart/chart.h \
    Model/barmodel.h \
    Model/linemodel.h \
    Model/model.h \
    Model/piemodel.h \
    View/CentralWidget/ChartWidget/barchartwidget.h \
    View/CentralWidget/ChartWidget/chartwidget.h \
    View/CentralWidget/ChartWidget/linechartwidget.h \
    View/CentralWidget/ChartWidget/piechartwidget.h \
    View/CentralWidget/centralwidget.h \
    View/view.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
