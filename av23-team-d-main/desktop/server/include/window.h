#ifndef WINDOW_H
#define WINDOW_H

#include <QLabel>
#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include "comservice.h"

class Window : public QDialog
{
    COMService &comservice;

public:
    Window(COMService &comsrvc);

private:
    QVBoxLayout mainLayout;
    QGridLayout sliderLayout;
    QGridLayout checkboxLayout;

    QSlider speedSlider{Qt::Horizontal};
    QSlider temperatureSlider{Qt::Horizontal};
    QSlider batterySlider{Qt::Horizontal};

    QCheckBox left{"Left"};
    QCheckBox right{"Right"};
    QCheckBox warning{"Warning"};

    QLabel speedLabel{"Speed:"};
    QLabel temperatureLabel{"Temperature:"};
    QLabel batteryLabel{"Battery level:"};
    QLabel lightLabel{" Light signals:"};

    QLabel speedValue{"0 kph"};
    QLabel temperatureValue{"0 °C"};
    QLabel batteryValue{"0%"};
};

#endif