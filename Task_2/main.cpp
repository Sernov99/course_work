
#include <QApplication>
#include <QtDataVisualization/Q3DSurface>
#include <QHBoxLayout>
#include <QWidget>
#include <QtGui/QScreen>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include<QMainWindow>
#include<QScrollArea>
#include "surfacegraph.h"
#include "mathhandler.h"
#include "splinechart.h"


void displayGrid(QSize screenSize );
void displayChart();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QtDataVisualization::Q3DSurface *graph = new QtDataVisualization::Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(graph);


    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);


    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);


    widget->setWindowTitle(QStringLiteral("Схема Кранка-Николсона"));

    QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("График"));

    QRadioButton *ExactRB = new QRadioButton(widget);
    ExactRB->setText(QStringLiteral("Точное"));
    ExactRB->setChecked(false);

    QRadioButton *ApproxRB = new QRadioButton(widget);
    ApproxRB->setText(QStringLiteral("Приближенное"));
    ApproxRB->setChecked(false);

    QVBoxLayout *modelVBox = new QVBoxLayout;
     modelVBox->addWidget(ExactRB);
     modelVBox->addWidget(ApproxRB);
     modelGroupBox->setLayout(modelVBox);

     vLayout->addWidget(modelGroupBox);

     QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);
       axisMinSliderX->setMinimum(0);
       axisMinSliderX->setTickInterval(0.1);
       axisMinSliderX->setEnabled(true);
       QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);
       axisMaxSliderX->setMinimum(0.2);
       axisMaxSliderX->setTickInterval(0.1);
       axisMaxSliderX->setEnabled(true);
       QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
       axisMinSliderZ->setMinimum(0);
       axisMinSliderZ->setTickInterval(0.1);
       axisMinSliderZ->setEnabled(true);
       QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
       axisMaxSliderZ->setMinimum(0.2);
       axisMaxSliderZ->setTickInterval(0.1);
       axisMaxSliderZ->setEnabled(true);

       vLayout->addWidget(new QLabel(QStringLiteral("Масштаб по х")));
       vLayout->addWidget(axisMinSliderX);
       vLayout->addWidget(axisMaxSliderX);
       vLayout->addWidget(new QLabel(QStringLiteral("Масштаб по z")));
       vLayout->addWidget(axisMinSliderZ);
       vLayout->addWidget(axisMaxSliderZ);

       widget->show();


    SurfaceGraph *modifier = new SurfaceGraph(graph);

    QObject::connect(ExactRB, &QRadioButton::toggled,
                       modifier, &SurfaceGraph::fillExact);
    QObject::connect(ApproxRB, &QRadioButton::toggled,
                       modifier, &SurfaceGraph::fillApprox);
    QObject::connect(axisMinSliderX, &QSlider::valueChanged,
                        modifier, &SurfaceGraph::adjustXMin);
    QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
                        modifier, &SurfaceGraph::adjustXMax);
    QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
                        modifier, &SurfaceGraph::adjustZMin);
    QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
                        modifier, &SurfaceGraph::adjustZMax);

    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);

    ApproxRB->setChecked(true);

    displayChart();
    displayGrid(screenSize);


    return app.exec();
}

void displayChart(){
        SplineChart *chart = new SplineChart();
        chart->show();
}


void displayGrid(QSize screenSize){

    //Get a grid!
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    QWidget *mainWidget = new QWidget(scrollArea);
    scrollArea->setWidget(mainWidget);

    QGridLayout *layout = new QGridLayout;
    mainWidget->setWindowTitle(QStringLiteral("Значения 'Явная схема'"));
    mainWidget->setMinimumSize(QSize(screenSize.width(), screenSize.height()));
    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWidget->setFocusPolicy(Qt::StrongFocus);

    MathHandler *m = new MathHandler();
    m->Approximate();


    //погрешности
    double absolute_last = 0;
    double approx_last = 0;
    double absolute_now = 0;
    double approx_now = 0;
    int x =0; int y=0;

    for (int i = 0; i < m->X_Y_Z_W_ExW->size(); i++) {
           absolute_now=m->X_Y_Z_W_ExW->value(i).w() - m->X_Y_Z_W_ExW->value(i).z();
           approx_now  =(m->X_Y_Z_W_ExW->value(i).w() - m->X_Y_Z_W_ExW->value(i).z())/m->X_Y_Z_W_ExW->value(i).w() *100;
            if(fabs(absolute_now) > fabs(absolute_last)) {absolute_last = absolute_now;};
            if( fabs(approx_now) > fabs(approx_last))  {approx_last = approx_now;};

            if( m->X_Y_Z_W_ExW->value(i).x()>0.1 && m->X_Y_Z_W_ExW->value(i).x()<0.6 &&
                   m->X_Y_Z_W_ExW->value(i).y()>0.1 &&m->X_Y_Z_W_ExW->value(i).y()<1 ) {

                QString a = " T: "+QString::number(m->X_Y_Z_W_ExW->value(i).y()) +"  X:  "+
                        QString::number(m->X_Y_Z_W_ExW->value(i).x())
                        +"\n W: "+  QString::number(m->X_Y_Z_W_ExW->value(i).w() ) + "  ~W:  "+
                        QString::number(m->X_Y_Z_W_ExW->value(i).z() )
                        +"\n E: "+  QString::number(absolute_now )
                        + "  E%:  "+
                        QString::number(approx_now) + "%";

                QLabel *label = new QLabel(a, mainWidget);
                 label->setStyleSheet("QLabel { background-color :white ; color : black; font-size:12px; }");
                layout->addWidget(label, x, y);
                y++;
                if(y==5) {y=0;x++;}
        }

    }


    QString a ="\n Абсолютная погрешность:\n    "
               +  QString::number(absolute_last )+ "\n Относительная погрешность:\n   "+ QString::number( approx_last) + "%";
    QLabel *label = new QLabel(a, mainWidget);
    label->setStyleSheet("QLabel { background-color : blue; color :white;  font: bold 14px;} ");
    layout->addWidget(label,x+1,y);

    mainWidget->setLayout(layout);
    mainWidget->show();
    mainWidget->setWindowTitle("Значения");
    scrollArea->show();
}
