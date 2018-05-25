#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>

using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
public:
    explicit SurfaceGraph(Q3DSurface *surface);
    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    void fillApprox(bool enable);
    void fillExact(bool enable);

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

     ~SurfaceGraph();
private:
    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;
    Q3DSurface *m_graph;
    QSurfaceDataProxy *m_Proxy;
    QSurface3DSeries *m_Series;
    QSurfaceDataProxy *m_ExactProxy;
    QSurface3DSeries *m_ExactSeries;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

};

#endif // SURFACEGRAPH_H
