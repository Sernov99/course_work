#include "surfacegraph.h"
#include "mathhandler.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;


SurfaceGraph::SurfaceGraph(Q3DSurface *surface)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_Proxy = new QSurfaceDataProxy();
    m_Series = new QSurface3DSeries(m_Proxy);
    m_ExactProxy = new QSurfaceDataProxy();
    m_ExactSeries = new QSurface3DSeries(m_ExactProxy);


}

SurfaceGraph:: ~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::fillApprox(bool enable)
{

    if (enable){
        //Добавляем сириес из мат.хэндлера
        MathHandler *m = new MathHandler();
        m->Approximate();
        QSurfaceDataArray *dataArray = m->dataArray;
        m_Proxy->resetArray(dataArray);
        m_Series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_Series->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(0, 1);
        m_graph->axisY()->setRange(0.0f, 1.000f);
        m_graph->axisZ()->setRange(0, 1);

        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);

        m_graph->removeSeries(m_ExactSeries);
        m_graph->addSeries(m_Series);


        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setValue(1);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setValue(1);


        //Градиентик
        QLinearGradient gr;
        gr.setColorAt(0.0, Qt::black);
        gr.setColorAt(0.33, Qt::blue);
        gr.setColorAt(0.67, Qt::red);
        gr.setColorAt(1.0, Qt::yellow);

        m_graph->seriesList().at(0)->setBaseGradient(gr);
        m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    }
}

void SurfaceGraph::fillExact(bool enable)
{
    if(enable){
        //Добавляем сириес из мат.хэндлера
        MathHandler *m = new MathHandler();
        m->Exact();
        QSurfaceDataArray *dataArray = m->dataArray;
        m_ExactProxy->resetArray(dataArray);
        m_ExactSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_ExactSeries->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);
        m_graph->axisX()->setRange(0, 1);
        m_graph->axisY()->setRange(0.0f, 1.0f);
        m_graph->axisZ()->setRange(0, 1);

        m_graph->removeSeries(m_Series);
        m_graph->addSeries(m_ExactSeries);

        //Градиентик
        QLinearGradient gr;
        gr.setColorAt(0.0, Qt::black);
        gr.setColorAt(0.33, Qt::blue);
        gr.setColorAt(0.67, Qt::red);
        gr.setColorAt(1.0, Qt::yellow);

        m_graph->seriesList().at(0)->setBaseGradient(gr);
        m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    }
}
void SurfaceGraph::adjustXMin(int min)
{
    float minX = 1 * float(min) ;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = 1 * max ;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    float maxX =float(max) ;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = min ;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    float minZ = float(min) ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = max ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    float maxX = float(max) ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX =  min ;

    setAxisZRange(minX, maxX);
}

void SurfaceGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}
