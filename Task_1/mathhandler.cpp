#include "mathhandler.h"
#include "qmath.h"
#include <QDebug>
#include <QList>


MathHandler::MathHandler()
{

}

void MathHandler::Approximate(){
    double x=0;
    double rowSteps = (1/h) +1;

    QList <double>*lastLayer= new QList <double>();
    QList <double>*currentLayer= new QList <double>();

    dataArray = new QtDataVisualization::QSurfaceDataArray;

    //Нулевой слой
    QtDataVisualization:: QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(rowSteps);
    int index = 0;
    while(x<=1){

        //Начальные условия
         lastLayer->append(pow(const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2));

         (*newRow)[index++].setPosition(QVector3D(x,0,pow(const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2)));

         currentLayer->append(0);
         x+=h;
     }
     *dataArray << newRow;


     //Остальные слои
     for(double i =t; i<=1; i+=t){
         QtDataVisualization:: QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(rowSteps);

         //граничные условия
         currentLayer->replace(0,pow(a*i+const_A,0.75)*pow((0+const_B)*(0+const_B+1),-3/(double)2));
         currentLayer->replace(currentLayer->size()-1,pow(a*i+const_A,0.75)*pow((1+const_B)*(1+const_B+1),-3/(double)2));

         (*newRow)[0].setPosition(QVector3D(0,i,currentLayer->value(0)));
         (*newRow)[currentLayer->size()-1].setPosition(QVector3D(1,i,currentLayer->value(currentLayer->size()-1)));

         x=h;
         int j = 1;index = 1;
         while(x<1){


             /*-----------------Примерная формула----------------*/
            currentLayer->replace(j,lastLayer->value(j) + a*t*( -4/(double)3 * pow(lastLayer->value(j), -7/(double)3) *
                                                          pow((lastLayer->value(j+1) - lastLayer->value(j-1))/(2*h),2) +
                                                          pow(lastLayer->value(j), -4/(double)3)  *
                                                          (lastLayer->value(j-1) -2*lastLayer->value(j)
                                                           + lastLayer->value(j+1))/(h*h)
                                                          )
                                  );

             (*newRow)[index++].setPosition(QVector3D(x,i, currentLayer->value(j)));

             j++;
             x+=h;


         }

         *dataArray << newRow;

         *lastLayer=*currentLayer;

     }

}

void MathHandler::Exact(){
    double rowSteps = (1/h)+1;

    double x =0;

    QList <double>*currentLayer= new QList <double>();
    dataArray = new QtDataVisualization::QSurfaceDataArray;

    //Забил место,чёт reserve не работает?

    int index = 0;
    while(x<=1){
         currentLayer->append(0);
         x+=h;
     }

     // слои
     for(double i =0; i<=1; i+=t){
         QtDataVisualization:: QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(rowSteps );

         x=0;
         int j = 0;index = 0;
         while(x<=1){

             /*-----------------Точная формула----------------*/
             currentLayer->replace(j,pow(a*i+const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2));

             (*newRow)[index++].setPosition(QVector3D(x,i, currentLayer->value(j)));

             j++;
             x+=h;
         }

         *dataArray << newRow;

     }

}
