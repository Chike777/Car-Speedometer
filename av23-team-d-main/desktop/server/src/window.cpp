#include "window.h"

Window::Window(COMService &comsrvc) : comservice{comsrvc}
{
    setWindowTitle("Server");
    resize(800, 150);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Speed
    speedValue.setFixedWidth(50);
    sliderLayout.addWidget(&speedLabel, 0, 0, Qt::Alignment{2});
    sliderLayout.addWidget(&speedSlider, 0, 1);
    sliderLayout.addWidget(&speedValue, 0, 2);
    speedSlider.setRange(0, 240);

    // Temperature
    temperatureValue.setFixedWidth(50);
    sliderLayout.addWidget(&temperatureLabel, 1, 0, Qt::Alignment{2});
    sliderLayout.addWidget(&temperatureSlider, 1, 1);
    sliderLayout.addWidget(&temperatureValue, 1, 2);
    temperatureSlider.setRange(-60, 60);

    // Battery
    batteryValue.setFixedWidth(50);
    sliderLayout.addWidget(&batteryLabel, 2, 0, Qt::Alignment{2});
    sliderLayout.addWidget(&batterySlider, 2, 1);
    sliderLayout.addWidget(&batteryValue, 2, 2);
    batterySlider.setRange(0, 100);

    mainLayout.addLayout(&sliderLayout);

    // Light signals
    checkboxLayout.addWidget(&lightLabel, 0, 0);
    checkboxLayout.addWidget(&left, 0, 1);
    checkboxLayout.addWidget(&right, 0, 2);
    checkboxLayout.addWidget(&warning, 0, 3);

    mainLayout.addLayout(&checkboxLayout);

    // Update checkbox states
    auto updateCheckbox = [this]()
    {
        if (left.isChecked())
        {
            right.setEnabled(false);
        }
        else if (right.isChecked())
        {
            left.setEnabled(false);
        }
        else if (warning.isChecked())
        {
            left.setEnabled(true);
            right.setEnabled(true);
        }
        else
        {
            left.setEnabled(true);
            right.setEnabled(true);
            warning.setEnabled(true);
        }
        comservice.set_left_signal(left.isChecked());
        comservice.set_right_signal(right.isChecked());
        comservice.set_warning_signal(warning.isChecked());
    };

    // Connect checkboxes
    connect(&left, &QCheckBox::stateChanged, this, updateCheckbox);
    connect(&right, &QCheckBox::stateChanged, this, updateCheckbox);
    connect(&warning, &QCheckBox::stateChanged, this, updateCheckbox);

    // Update value labels with actual value
    connect(&speedSlider, &QSlider::valueChanged, this, [this](int value)
            { speedValue.setText(QString::number(value) + " kph");
            comservice.set_speed(value); });

    connect(&temperatureSlider, &QSlider::valueChanged, this, [this](int value)
            { temperatureValue.setText(QString::number(value) + " °C");
            comservice.set_temperature(value); });

    connect(&batterySlider, &QSlider::valueChanged, this, [this](int value)
            { batteryValue.setText(QString::number(value) + " %"); 
            comservice.set_battery_level(value); });

    // Set layout
    setLayout(&mainLayout);
}