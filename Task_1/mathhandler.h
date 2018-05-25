#ifndef MATHHANDLER_H
#define MATHHANDLER_H
#include <QtDataVisualization/Q3DSurface>

class MathHandler
{
public:
    MathHandler();
    void Approximate();
    void Exact();
    ~MathHandler();
    QtDataVisualization::QSurfaceDataArray *dataArray;
private:
    const double const_A =2;
    const double const_B = 2;

    double h = 0.25;
    double t = 1/(double)32;
    double a =0.01;
};

#endif // MATHHANDLER_H
