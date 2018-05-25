#include "mathhandler.h"
#include "qmath.h"
#include <QDebug>
#include <QList>


MathHandler::MathHandler()
{

}

void MathHandler::Approximate(){

    double r=(1/h_max)+1;
    int rowSteps = r;

    QList <double>*lastLayer= new QList <double>();
    QList <double>*currentLayer= new QList <double>();

    X_Y_Z_W_ExW = new QList <QVector4D>;
    dataArray = new QtDataVisualization::QSurfaceDataArray;
    ExactdataArray = new QtDataVisualization::QSurfaceDataArray;

    double **A = new double* [rowSteps];
    for (int count = 0; count < rowSteps; count++) A[count] = new double[rowSteps];
    double *B = new double [rowSteps];
    double *X= new double[rowSteps];

    double x=0;

    bool resetLayer;

    // начальные условия
    while(x<=1){
         lastLayer->append(pow(const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2));
         currentLayer->append(pow(const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2));
         x+=h;
     }
    //Отобразим слой - при необходимости интерполируем
     *dataArray << lagranzDisplayInterpolation(lastLayer,0);

     for(double tau =t; tau<=1;tau+=t){

             //-------------------Метод Кранка-Николсона-------------------------
              for (int count = 0; count <100; count++){

                 double max_delta_x =0;
                 //Заполнили матрицы
                 fillMatrix(A,B,rowSteps,lastLayer,currentLayer,tau);
                 //решили систему
                 X=systemSolver(A,B,rowSteps);

                 //Нужны ли еще итерации?
                 for (int c = 0; c < rowSteps; c++){
                    currentLayer->replace(c,currentLayer->value(c)+X[c]);
                    if(max_delta_x<X[c]) max_delta_x = X[c];
                 }
                 if (max_delta_x< e) count=101;
              }


              //перейдем на этот же слой с h/2 t/2;
              resetLayer =false;
              QList <double>*HalfTauHalfHCurrentLayer= OnThisLayer_HalfTauHalfH(lastLayer,tau);

              //проверим необходимость сгущения сетки
              for(int i =0,q=0; i<rowSteps; i++,q+=2){
                  if(fabs(1/(double)3*fabs(currentLayer->value(i)-HalfTauHalfHCurrentLayer->value(q))) >e)

                      if(h/2 > h_min && t/2 > t_min){

                          //сгущенная опорная функция
                          lastLayer =InterpolateLayer(lastLayer,h/2);
                          *currentLayer=*lastLayer;

                          h/=2;
                          t/=2;
                          tau-=t;

                          r=(1/h)+1;
                          rowSteps = r;

                          //TODO:освободить память

                          A = new double* [rowSteps];
                          for (int count = 0; count < rowSteps; count++) A[count] = new double[rowSteps];
                          B = new double [rowSteps];
                          X= new double[rowSteps];

                          resetLayer = true;
                      }
              }

              //шаг принят
              if(!resetLayer){
                  //Отобразим слой - при необходимости интерполируем
                  *dataArray << lagranzDisplayInterpolation(currentLayer,tau);

                  //новые шаги
                  double *alpha = new double;
                  double *beta = new double;
                  calculateAlpa_Beta(alpha,beta,lastLayer,lastLayer,tau);

                  if(h*(*alpha) <h_max && t*(*beta) < t_max){
                      //новая опорная функция
                      currentLayer =InterpolateLayer(currentLayer,h*(*alpha));
                      h*=(*alpha);
                      t*=(*beta);

                      r=(1/h)+1;
                      rowSteps = r;

                      //TODO:освободить память
                      A = new double* [rowSteps];
                      for (int count = 0; count < rowSteps; count++) A[count] = new double[rowSteps];
                      B = new double [rowSteps];
                      X= new double[rowSteps];

                 }
                 *lastLayer=*currentLayer;
         }

     }
}

void MathHandler::Exact(){

    double r=(1/h)+1;
    int rowSteps = r;

    double x =0;
    dataArray = new QtDataVisualization::QSurfaceDataArray;

     // слои
     for(double i =0; i<=1+t; i+=t){
         QtDataVisualization:: QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(rowSteps );

         x=0;
         int index =0;
         while(x<=1){

             /*-----------------Точная формула----------------*/

             (*newRow)[index++].setPosition(QVector3D(x,i,pow(a*i+const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2)));

             x+=h;
         }

         *dataArray << newRow;

     }

}

double * MathHandler::systemSolver(double **a, double *y, int n)
{
    int i,k,n1;
    double z;
    double eps[n];
    double *x = new double [n];
    double et[n];

    n1=n-1;
    eps[0]=-a[0][1]/a[0][0];
    et[0]=y[0]/a[0][0];

    for(i=1;i<n1;i++)
        {
        z=a[i][i]+a[i][i-1]*eps[i-1];
        eps[i]=-a[i][i+1]/z;
        et[i]=(y[i]-a[i][i-1]*et[i-1])/z;
        }

    x[n1]=(y[n1]-a[n1][n1-1]*et[n1-1])/(a[n1][n1]+a[n1][n1-1]*eps[n1-1]);

    for(i=n1-1;i>=0;i--)
        x[i]=eps[i]*x[i+1]+et[i];


    return x;

}

void MathHandler::fillMatrix(double **A, double *B, int sz,  QList <double>*lastLayer, QList<double> *currentLayer, double tau){

    //Обнуляем матрицу
    for (int i=0;i<sz;i++) for (int k=0;k<sz;k++){A[i][k] =0; }

    //границы
    A[0][0] =1;
    A[sz-1][sz-1] =1;
    B[0] = pow(a*tau+const_A,0.75)*pow((0+const_B)*(0+const_B+1),-3/(double)2)-currentLayer->value(0) ;

    B[sz-1] = pow(a*tau+const_A,0.75)*pow((1+const_B)*(1+const_B+1),-3/(double)2)-currentLayer->value(sz-1);

    //Заполняем матрицу
         for ( int i=1;i<sz-1;i++){
             for (int k=0;k<sz-1;k++){
                 if(i==k){
                    //a2
                    A[i][k] =  -1 +a*(7/(double)9 * pow(currentLayer->value(i),-10/(double)3)*
                            pow((currentLayer->value(i+1)-currentLayer->value(i-1))/h,2) +8/(double)3*pow(currentLayer->value(i),-7/(double)3)/(h*h));

                    //a3
                    if((k+1) <sz)  A[i][k+1] =a*(2/(double)3 *pow(currentLayer->value(i),-7/(double)3)/(h*h)
                                *(currentLayer->value(i+1)-currentLayer->value(i-1)) +pow(currentLayer->value(i),-4/(double)3)/(h*h));   ;

                    //a1
                    if((k-1) >-1) A[i][k-1] = a*(-2/(double)3*((currentLayer->value(i+1)- currentLayer->value(i-1)))*
                                (pow(currentLayer->value(i),-7/(double)3)/(h*h))+pow(currentLayer->value(i),-4/(double)3)/(h*h));

                 }
            }
        }

         for (int i=1;i<sz-1;i++){
             B[i] =  lastLayer->value(i) -currentLayer->value(i) +(a*t)/2*(
                         (-2/3 * pow(lastLayer->value(i),-7/3) *
                          pow((lastLayer->value(i+1)-lastLayer->value(i-1))/h,2) +
                             pow(lastLayer->value(i),-4/3) *
                            ((lastLayer->value(i+1) - 2*lastLayer->value(i)+lastLayer->value(i-1))/(h*h)) )+

                         ( -2/3 * pow(currentLayer->value(i),-7/3) *
                         pow((currentLayer->value(i+1)-currentLayer->value(i-1))/h,2) +
                            pow(currentLayer->value(i),-4/3) *
                           ((currentLayer->value(i+1) - 2*currentLayer->value(i)+currentLayer->value(i-1))/(h*h)) )

                           );

                     ;
         }

}

QtDataVisualization:: QSurfaceDataRow * MathHandler::lagranzDisplayInterpolation(QList<double> *Layer,double tau){
 //перед этим запомним слой в 4д вектор
    double x=0;int i =0;
    while(x<=1){
        X_Y_Z_W_ExW->append(QVector4D(x,tau,Layer->value(i),
                              pow(a*tau+const_A,0.75)*pow((x+const_B)*(x+const_B+1),-3/(double)2)));
        x+=h;
        i++;
    }

    double r=(1/h_min)+1;
    int rowSteps = r;

    int index=0;
    int indexExact=0;
    double x_interpolated =0;
    x=0;
    double z=0;

    double Y[Layer->size()];
    double X[Layer->size()];
    for(int i=0; i<Layer->size();i++){
        Y[i] = Layer->value(i);
        X[i] =x;
        x+=h;
    }

    QtDataVisualization:: QSurfaceDataRow *newRow = new QtDataVisualization::QSurfaceDataRow(rowSteps);
    QtDataVisualization:: QSurfaceDataRow *ExactnewRow = new QtDataVisualization::QSurfaceDataRow(rowSteps);


    for(double c=0; c<rowSteps;c++){

        //Если точки такой нет - интерполируем
            double p1,p2;
            z=0;
            for (int j=0; j<Layer->size(); j++){
                    p1=1; p2=1;
                    for (int i=0; i<Layer->size(); i++){
                            if (i==j){
                                p1=p1*1;p2=p2*1;
                            }
                            else {
                                    p1=p1*( x_interpolated-X[i]);
                                    p2=p2*(X[j]-X[i]);
                        }
                    }
                    z=z+Y[j]*p1/p2;
            }

            (*newRow)[index++].setPosition(QVector3D(x_interpolated,tau,z));

            (*ExactnewRow)[indexExact++].setPosition(QVector3D(x_interpolated,tau,pow(a*tau+const_A,0.75)*
                                                          pow((x_interpolated+const_B)*(x_interpolated+const_B+1),-3/(double)2)));

            x_interpolated+=h_min;
    }

    *ExactdataArray<<ExactnewRow;
    return newRow;
}

QList<double> * MathHandler::OnThisLayer_HalfTauHalfH(QList <double>*lastLayer, double tau){

    double r=(1/(h/2))+1;
    int rowSteps = r;

    QList <double>*lastLayerInterpolated;
    QList <double>* result = new QList <double>();

    lastLayerInterpolated = InterpolateLayer(lastLayer,h/2);

    *result=*lastLayerInterpolated;

    double save_h =h;
    double save_t =t;
    h/=2;
    t/=2;
    double **A = new double* [rowSteps];
    for (int count = 0; count < rowSteps; count++) A[count] = new double[rowSteps];
    double *B = new double [rowSteps];
    double *W= new double[rowSteps];

    //-------------------Метод Кранка-Николсона-------------------------
     for (int count = 0; count <100; count++){
        double max_delta_x =0;
        //Заполнили матрицы
        fillMatrix(A,B,rowSteps,lastLayerInterpolated,result,tau - t);
        //решили систему
        W=systemSolver(A,B,rowSteps);

        //Нужны ли еще итерации?
        for (int c = 0; c < rowSteps; c++){
           result->replace(c,result->value(c)+W[c]);
           if(max_delta_x<W[c]) max_delta_x = W[c];
        }
        if (max_delta_x< e) count=101;
     }

    *lastLayerInterpolated = *result;

      //-------------------Метод Кранка-Николсона-------------------------
     for (int count = 0; count <100; count++){
        double max_delta_x =0;
        //Заполнили матрицы
        fillMatrix(A,B,rowSteps,lastLayerInterpolated,result,tau);
        //решили систему
        W=systemSolver(A,B,rowSteps);

        //Нужны ли еще итерации?
        for (int c = 0; c < rowSteps; c++){
           result->replace(c,result->value(c)+W[c]);
           if(max_delta_x<W[c]) max_delta_x = W[c];
        }
        if (max_delta_x< e) count=101;
     }

    delete lastLayerInterpolated ;
    delete B;
    delete W;

     t=save_t;
     h=save_h;

     return result;
}

void MathHandler::calculateAlpa_Beta(double*alpha, double*beta, QList <double>*lastLayer,QList<double> *currentLayer, double tau){

    double r=(1/(h/2))+1;
    int rowSteps = r;

    QList <double>*lastLayerInterpolated= InterpolateLayer(lastLayer,h/2);
    QList <double>* result = new QList <double>();

    *result=*lastLayerInterpolated;

    double save_h =h;
    h/=2;

    double **A = new double* [rowSteps];
    for (int count = 0; count < rowSteps; count++) A[count] = new double[rowSteps];
    double *B = new double [rowSteps];
    double *W= new double[rowSteps];

    //-------------------Метод Кранка-Николсона-------------------------
     for (int count = 0; count <100; count++){
        double max_delta_x =0;
        //Заполнили матрицы
        fillMatrix(A,B,rowSteps,lastLayerInterpolated,result,tau - t);
        //решили систему
        W=systemSolver(A,B,rowSteps);

        //Нужны ли еще итерации?
        for (int c = 0; c < rowSteps; c++){
           result->replace(c,result->value(c)+W[c]);
           if(max_delta_x<W[c]) max_delta_x = W[c];
        }
        if (max_delta_x< e) count=101;
     }

    delete lastLayerInterpolated ;
    delete B;
    delete W;
    h=save_h;

    QList <double>*HalfTauHalfHLayer= OnThisLayer_HalfTauHalfH(lastLayer,tau);

    double curr_alpha ;
    double curr_beta;
    double last_alpha= alpha_max;
    double last_beta=beta_max;

    for(int i=0,q=0; i<currentLayer->size() && q<HalfTauHalfHLayer->size();i++,q+=2){
        curr_alpha =pow((3*e)/(8*(result->value(q) - currentLayer->value(i))),1/(double)3);


        if(curr_alpha > alpha_min && curr_alpha < last_alpha) {last_alpha = curr_alpha;}

        curr_beta =sqrt( (3*( e - ( 4/(3*t*t*t) )*( HalfTauHalfHLayer->value(q) - result->value(q) ) ) )
                         /(4*curr_alpha  *(result->value(q) - currentLayer->value(i)) ) );

        if(curr_beta > beta_min && curr_beta < last_beta) last_beta = curr_beta;


    }

 *(alpha) = last_alpha;
 *(beta)=last_beta;

}

QList<double> *MathHandler::InterpolateLayer(QList <double>*Layer, double new_h){

    double r=(1/(new_h))+1;
    int rowSteps = r;

    double x_interpolated =0;
    double x=0;
    double z=0;

    QList <double>*lastLayerInterpolated= new QList <double>();

    double Y[Layer->size()];
    double X[Layer->size()];
    for(int i=0; i<Layer->size();i++){
        Y[i] = Layer->value(i);
        X[i] =x;
        x+=h;
    }

    //опорная функция с шагом h/2
    for(double c=0; c<rowSteps;c++){
        //Если точки такой нет - интерполируем
            double p1,p2;
            z=0;
            for (int j=0; j<Layer->size(); j++){
                    p1=1; p2=1;
                    for (int i=0; i<Layer->size(); i++){
                            if (i==j){
                                p1=p1*1;p2=p2*1;
                            }
                            else {
                                    p1=p1*(x_interpolated-X[i]);
                                    p2=p2*(X[j]-X[i]);
                        }
                    }
                    z=z+Y[j]*p1/p2;
            }
            lastLayerInterpolated->append(z);
            x_interpolated+=new_h;
    }
    return lastLayerInterpolated;
}
