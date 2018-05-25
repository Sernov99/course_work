#ifndef MATHHANDLER_H
#define MATHHANDLER_H
#include <QtDataVisualization/Q3DSurface>
#include<QVector4D>

class MathHandler
{
public:
    MathHandler();

    QtDataVisualization::QSurfaceDataArray * dataArray;
    QtDataVisualization::QSurfaceDataArray * ExactdataArray;
    QList<QVector4D> *X_Y_Z_W_ExW;

    void Approximate();
    void Exact();
    ~MathHandler();


private:
    const double const_A =2;
    const double const_B = 2;
    const double a =0.01;

    double e =0.01;
    double h = 0.1;
    double h_min =0.04;
    double h_max =0.25;
    double t_min=0.001;
    double t_max=0.2;
    double t = 0.04;
    double alpha_min=0.5,beta_min =0.5;
    double alpha_max=4,beta_max =4;


    QtDataVisualization:: QSurfaceDataRow *lagranzDisplayInterpolation(QList<double> *Layer,double tau);
    QList<double> *OnThisLayer_HalfTauHalfH(QList <double>*lastLayer,double tau);
    QList<double> *InterpolateLayer(QList <double>*Layer,double new_h);
    void calculateAlpa_Beta(double*alpha,double*beta,QList <double>*lastLayer,QList<double> *currentLayer,double tau);
    double * systemSolver(double **a, double *y, int n) ;
    void fillMatrix(double **A, double *B, int sz,  QList <double>*lastLayer, QList<double> *currentLayer, double tau);

};

#endif // MATHHANDLER_H
