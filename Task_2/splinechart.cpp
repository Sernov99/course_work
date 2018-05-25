#include "splinechart.h"

#include<QPushButton>
#include <QVBoxLayout>
#include<QGroupBox>
#include <QString>


SplineChart::SplineChart(QWidget *parent)
    : QWidget(parent)

{

    this->setWindowTitle("2D График");
    this->setFixedSize(600,600);

    m = new MathHandler();
    m->Approximate();

    t =0;

    series1 = new  QtCharts::QSplineSeries();
    series2 = new  QtCharts::QSplineSeries();
    for(int i =0; i<m->dataArray->value(t)->size();i++){
       *series1 << QPointF(m->dataArray->value(t)->value(i).x(),m->dataArray->value(t)->value(i).z());
       *series2 << QPointF(m->ExactdataArray->value(t)->value(i).x(),m->ExactdataArray->value(t)->value(i).z());
    }


    chart = new  QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(series1);
    chart->addSeries(series2);

    chart->setTitle("TAU = 0");
    chart->createDefaultAxes();
   // chart->axisY()->setRange(0, 0.125);
   // chart->axisX()->setRange(0, 1);


   QtCharts::QChartView *chartView = new  QtCharts::QChartView(chart);
   chartView->setRenderHint(QPainter::Antialiasing);


   QVBoxLayout *mainLayout = new QVBoxLayout;

   mainLayout->addWidget(chartView);

   QGroupBox* horizontalGroupBox = new QGroupBox();

   QHBoxLayout *Buttonslayout = new QHBoxLayout;
   QPushButton *forward = new QPushButton(">>>>");
   QPushButton *backward = new QPushButton("<<<<");

   Buttonslayout->addWidget(backward);
   Buttonslayout->addWidget(forward);

   horizontalGroupBox->setLayout(Buttonslayout);

   mainLayout->addWidget(horizontalGroupBox);

   this->setLayout(mainLayout);

   connect(forward, SIGNAL(clicked()), this, SLOT(next()));
   connect(backward, SIGNAL(clicked()), this, SLOT(prev()));


}



void SplineChart::next(){
    if(m->dataArray->value(t)->value(0).y() == 1)return;
    t++;
    chart->removeSeries(series1);
    chart->removeSeries(series2);

    series1 = new  QtCharts::QSplineSeries();
    series2 = new  QtCharts::QSplineSeries();
    for(int i =0; i<m->dataArray->value(t)->size();i++){
       *series1 << QPointF(m->dataArray->value(t)->value(i).x(),m->dataArray->value(t)->value(i).z());
       *series2 << QPointF(m->ExactdataArray->value(t)->value(i).x(),m->ExactdataArray->value(t)->value(i).z());
    }
     chart->addSeries(series1);
      chart->addSeries(series2);
     chart->setTitle("TAU = "+QString::number(m->dataArray->value(t)->value(0).y()));


}
void SplineChart::prev(){
    if(m->dataArray->value(t)->value(0).y() == 0)return;
    t--;
    chart->removeSeries(series1);
    chart->removeSeries(series2);

    series1 = new  QtCharts::QSplineSeries();
    series2 = new  QtCharts::QSplineSeries();
    for(int i =0; i<m->dataArray->value(t)->size();i++){
       *series1 << QPointF(m->dataArray->value(t)->value(i).x(),m->dataArray->value(t)->value(i).z());
       *series2 << QPointF(m->ExactdataArray->value(t)->value(i).x(),m->ExactdataArray->value(t)->value(i).z());
    }

      chart->setTitle("TAU = "+QString::number(m->dataArray->value(t)->value(0).y()));
      chart->addSeries(series1);
      chart->addSeries(series2);

}
