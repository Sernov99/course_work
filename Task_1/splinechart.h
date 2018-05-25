#ifndef SPLINECHART_H
#define SPLINECHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include "mathhandler.h"

 class SplineChart : public QWidget
 {
     Q_OBJECT
 private:
     double t;

     QtCharts::QSplineSeries *series1;
     QtCharts::QSplineSeries *series2 ;
     MathHandler *m ;
     MathHandler *n ;
     QtCharts:: QChart *chart;
 public:
     SplineChart(QWidget *parent = 0);
 public slots:
     void next();
     void prev();
 };

#endif // SPLINECHART_H
