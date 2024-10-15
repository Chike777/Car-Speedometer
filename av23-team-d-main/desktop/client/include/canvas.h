#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QFontDatabase>
#include <QString>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDebug>

class Canvas : public QWidget
{
private:
    int class_speed = 0;
    int temperature = 0;
    int battery_percentage = 0;
    bool connection = false;
    bool blinker_left = false;
    bool blinker_right = false;
    bool blinker_warning = false;

    bool blinker_visible = true;

    QPainter painter;

    // Initialize QMediaPlayer and QAudioOutput
    QMediaPlayer player;
    QAudioOutput audioOutput;

public:
    Canvas();

    void set_speed(int speed);

    void set_temperature(int temp);

    void set_battery_percentage(int charge);

    void set_connection_status(bool status);

    void set_blinker_left(bool mode);

    void set_blinker_right(bool mode);

    void set_blinker_warning(bool mode);

private:
    void paintEvent(QPaintEvent *) override;

    void paint_speed(int speed);

    void paint_battery(int charge);

    void paint_temperature(int temp);

    void paint_light_signal(bool left, bool right, bool warning);

    void paint_no_connection();
};

#endif