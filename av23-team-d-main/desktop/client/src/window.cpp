#include "window.h"

Window::Window(COMService &comsrv) : comservice{comsrv}
{
    setWindowTitle("client");
    setFixedSize(800, 560);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Set background color of the window
    setStyleSheet("background-color: rgb(60, 0, 70);");

    // Adjust layout margins
    layout.setContentsMargins(0, 0, 0, 0); // Set margins to zero

    layout.addWidget(&canvas); // Add the canvas directly to the layout
    setLayout(&layout);

    QObject::connect(&timer, &QTimer::timeout, this, [this]()
                     { this->updateCanvas(); });

    timer.start(Settings::INTERVAL);
}

void Window::updateCanvas()
{
    canvas.set_connection_status(comservice.get_status());
    canvas.set_speed(comservice.get_speed());
    canvas.set_battery_percentage(comservice.get_battery_level());
    canvas.set_temperature(comservice.get_temperature());
    canvas.set_blinker_left(comservice.get_left_signal());
    canvas.set_blinker_right(comservice.get_right_signal());
    canvas.set_blinker_warning(comservice.get_warning_signal());

    canvas.update();
}
