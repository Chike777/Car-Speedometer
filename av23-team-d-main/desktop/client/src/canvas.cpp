#include "canvas.h"
#include <QFileInfo>

Canvas::Canvas()
{
    // Set up the media player
    player.setAudioOutput(&audioOutput);

    QFileInfo file(__FILE__);
    QString dirPath = file.absolutePath() + "/../res/";

    // Load the sound file
    QUrl soundUrl = QUrl::fromLocalFile(dirPath + "turn-signals.wav");
    player.setSource(soundUrl);
    player.setLoops(QMediaPlayer::Infinite);
    //  Set the volume and start playing
    audioOutput.setVolume(0.5);
}

void Canvas::set_speed(int speed)
{
    class_speed = speed;
}

void Canvas::set_temperature(int temp)
{
    temperature = temp;
}

void Canvas::set_battery_percentage(int charge)
{
    battery_percentage = charge;
}

void Canvas::set_connection_status(bool status)
{
    connection = status;
}

void Canvas::set_blinker_left(bool mode)
{
    blinker_left = mode;
}

void Canvas::set_blinker_right(bool mode)
{
    blinker_right = mode;
}

void Canvas::set_blinker_warning(bool mode)
{
    blinker_warning = mode;
}

void Canvas::paintEvent(QPaintEvent *)
{
    painter.begin(this);

    painter.setRenderHint(QPainter::Antialiasing); // Enable antialiasing

    if (connection)
    {
        paint_speed(class_speed);
        paint_battery(battery_percentage);
        paint_temperature(temperature);
        paint_light_signal(blinker_left, blinker_right, blinker_warning);
    }
    else
    {
        paint_speed(0);
        paint_no_connection();
        paint_battery(0);
        paint_temperature(0);
    }

    painter.end();
}

void Canvas::paint_speed(int speed)
{
    QPointF center(350, 385);
    qreal radius = 300;
    // Set the pen width to make the arc line thicker
    painter.setPen(QPen(Qt::white, 10)); // Set pen color and width

    // Calculate the bounding rectangle of the circle
    QRectF circleRect(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius);

    // Calculate the start angle and span angle of the arc
    qreal startAngle = -30; // Start angle in degrees
    qreal spanAngle = 240;  // Span angle in degrees
    // Draw the arc
    painter.drawArc(circleRect, startAngle * 16, spanAngle * 16);

    // DRAWING THE INNER CIRCLE
    // Calculate the center point of the bounding rectangle
    QPointF centerOfCircleRect = circleRect.center();

    // Radius of the circle to be drawn in the center
    qreal centerCircleRadius = 20;

    // Calculate the bounding rectangle of the center circle
    QRectF centerCircleRect(centerOfCircleRect.x() - centerCircleRadius, centerOfCircleRect.y() - centerCircleRadius, 2 * centerCircleRadius, 2 * centerCircleRadius);

    // Set the brush color for the center circle
    painter.setBrush(Qt::red);

    // Draw the circle in the center
    painter.drawEllipse(centerCircleRect);

    // PAITING INFO ALONG THE ARC SECTION

    // Calculate the number of points along the arc
    int numPoints = 13; // Adjust as needed

    // Calculate the angular step size between points
    qreal angleStep = spanAngle / (numPoints - 1);

    // Draw pattern along the arc
    painter.setPen(QPen(Qt::white, 10)); // Set pen color and width

    // Distance to draw the line from the arc
    qreal lineLength = 30; // Adjust as needed

    for (int i = 0; i < numPoints; ++i)
    {
        // Calculate the angle for this point
        qreal angle = (startAngle + i * angleStep) * M_PI / 180.0;

        // Calculate the position of the point along the arc
        qreal x = center.x() + radius * cos(angle);
        qreal y = center.y() - radius * sin(angle);

        // Calculate the endpoint of the line
        qreal lineX = x - lineLength * cos(angle);
        qreal lineY = y + lineLength * sin(angle);

        // Draw a line from the point on the arc to the calculated endpoint
        painter.drawLine(QPointF(x, y), QPointF(lineX, lineY));
    }

    // MEDIUM LINES

    // Calculate the start angle and span angle of the arc
    startAngle = -20; // Start angle in degrees

    // Distance to draw the line from the arc
    lineLength = 25;

    // Calculate the number of points along the arc
    numPoints = 12;

    angleStep = spanAngle / 12;

    // Draw pattern along the arc
    painter.setPen(QPen(Qt::white, 8)); // Set pen color and width

    for (int i = 0; i < numPoints; ++i)
    {
        // Calculate the angle for this point
        qreal angle = ((startAngle + i * angleStep) * M_PI / 180.0);

        // Calculate the position of the point along the arc
        qreal x = center.x() + radius * cos(angle);
        qreal y = center.y() - radius * sin(angle);

        // Calculate the endpoint of the line
        qreal lineX = x - lineLength * cos(angle);
        qreal lineY = y + lineLength * sin(angle);

        // Draw a line from the point on the arc to the calculated endpoint
        painter.drawLine(QPointF(x, y), QPointF(lineX, lineY));
    }

    //////// Thinnest lines//////////

    // Calculate the start angle and span angle of the arc
    startAngle = -25; // Start angle in degrees

    // Distance to draw the line from the arc
    lineLength = 20; // Adjust as needed

    numPoints = 24; // Adjust as needed

    angleStep = spanAngle / 24;

    // Draw pattern along the arc
    painter.setPen(QPen(Qt::white, 4)); // Set pen color and width

    for (int i = 0; i < numPoints; ++i)
    {
        // Calculate the angle for this point
        qreal angle = ((startAngle + i * angleStep) * M_PI / 180.0);

        // Calculate the position of the point along the arc
        qreal x = center.x() + radius * cos(angle);
        qreal y = center.y() - radius * sin(angle);

        // Calculate the endpoint of the line
        qreal lineX = x - lineLength * cos(angle);
        qreal lineY = y + lineLength * sin(angle);

        // Draw a line from the point on the arc to the calculated endpoint
        painter.drawLine(QPointF(x, y), QPointF(lineX, lineY));
    }

    // TEXT ALONG ARC
    // Center point of the circle
    QPointF center2(327, 385);

    // Calculate the start angle and span angle of the arc
    startAngle = -34; // Start angle in degrees
    spanAngle = 247;  // Span angle in degrees

    // Calculate the number of points along the arc
    numPoints = 13; // Adjust as needed

    // Radius of the circle
    radius = 235; // Adjust radius for text placement

    // Calculate the angular step size between points
    angleStep = spanAngle / (numPoints - 1);

    // Array of text speed to place along the arc
    QString texts[] = {"240", "220", "200", "180", "160", "140", "120", "100", "80", "60", "40", "20", "0"};

    // Set font for the text
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.setPen(QPen(Qt::white)); // Set pen color for the text

    for (int i = 0; i < numPoints; ++i)
    {
        // Calculate the angle for this point
        qreal angle = (startAngle + i * angleStep) * M_PI / 180.0;

        // Calculate the position of the point along the arc
        qreal x = center2.x() + radius * cos(angle);
        qreal y = center2.y() - radius * sin(angle);

        // Draw the text at the calculated position
        painter.drawText(QPointF(x, y), texts[i]);
    }

    // SPEED NEEDLE SECTION

    // Arc parameters
    startAngle = -30; // Start angle in degrees
    spanAngle = 240;  // Span angle in degrees
    radius = 300;

    // Calculate the angle for this point
    qreal angle = (startAngle + (spanAngle - speed)) * M_PI / 180.0;

    // Set the desired length of the line (as a fraction of the radius)
    qreal lengthPercentage = 0.8; // For example, 0.5 means the line will be half the radius

    // Calculate the adjusted length of the line
    qreal adjustedLength = radius * lengthPercentage;

    // Calculate the endpoint of the line based on the adjusted length
    qreal lineX = center.x() + adjustedLength * cos(angle);
    qreal lineY = center.y() - adjustedLength * sin(angle);

    // Clear the area where the previous line was drawn MAYBE DONT NEED?
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawLine(QPointF(lineX, lineY), QPointF(center));
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.setPen(QPen(Qt::red, 10)); // Set pen color and width

    // Draw a line from the point on the arc to the calculated endpoint
    painter.drawLine(QPointF(lineX, lineY), QPointF(center));

    // TEXT UNDER AND SYMBOL
    QString speedText = QString::number(speed) + " km/h";    // Convert speed to string
    QRect speedRect = QRect(350 - 100, 385 + 100, 200, 40);  // Define the rectangle for the speed text
    painter.setPen(QPen(Qt::white, 10));                     // Set pen color and width
    painter.drawText(speedRect, Qt::AlignCenter, speedText); // Draw the speed text

    // Set font size for the icon
    QFont font("Material Icons", 50);
    painter.setFont(font);

    // Define the battery icon character from Material Icons font
    QString speed_icon = QChar(0xE9E4); // Replace with actual Unicode if different

    painter.drawText(QPointF(318, 495), speed_icon); // Adjust as needed
}

void Canvas::paint_battery(int charge)
{

    // Draw the battery icon
    if (charge < 25)
    {
        painter.setPen(QPen(Qt::red)); // Set pen color for the icon
        painter.setBrush(Qt::red);     // Set the fill color
    }
    else if (charge >= 25 && charge < 50)
    {
        painter.setPen(QPen(Qt::yellow)); // Set pen color for the icon
        painter.setBrush(Qt::yellow);     // Set the fill color
    }
    else
    {
        painter.setPen(QPen(Qt::green)); // Set pen color for the icon
        painter.setBrush(Qt::green);     // Set the fill color
    }

    // Set font size for the icon
    QFont font("Material Icons", 150);
    painter.setFont(font);

    // Define the battery icon character from Material Icons font
    QString batteryIcon = QChar(0xEBDC);              // Replace with actual Unicode if different
    painter.drawText(QPointF(625, 425), batteryIcon); // Adjust as needed

    int height = 120 * (0.01 * charge); // Height of the rectangle depening on percentage
    int bottomY = 393;                  // y-coordinate of the bottom-left corner
    // Calculate the y-coordinate of the top-left corner based on the bottom-left corner
    int y = bottomY - height;

    // Draw the rectangle
    painter.drawRect(700, y, 50, height);

    // PERCENTAGE!
    // Draw the battery percentage text below the icon
    painter.setPen(QPen(Qt::white)); // Set pen color for the icon
    QString batteryText = QString::number(charge) + "%";
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(QRect(625 + 80, 425 - 15, 50, 30), Qt::AlignCenter, batteryText);
}

void Canvas::paint_temperature(int temp)
{
    // first the colour is decided
    if (temp < 5)
    {
        painter.setPen(QPen(Qt::white)); // Set pen color for the icon
        painter.setBrush(Qt::white);     // Set the fill color
    }
    else if (temp >= 5 && temp < 40)
    {
        painter.setPen(QPen(Qt::blue)); // Set pen color for the icon
        painter.setBrush(Qt::blue);     // Set the fill color
    }
    else
    {
        painter.setPen(QPen(Qt::red)); // Set pen color for the icon
        painter.setBrush(Qt::red);     // Set the fill color
    }

    // Set font size for the icon
    QFont font2("Material Icons", 50);
    painter.setFont(font2);

    // Define the temperature icon character from Material Icons font
    QString temp_icon = QChar(0xF076);              // Replace with actual Unicode if different
    painter.drawText(QPointF(690, 510), temp_icon); // Adjust as needed

    // TEXT TEMP!!
    painter.setPen(QPen(Qt::white)); // Set pen color for the icon
    QString tempText = QString::number(temp) + "°C";
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(QRect(690 + 10, 510, 50, 30), Qt::AlignCenter, tempText);
}

void Canvas::paint_light_signal(bool left, bool right, bool warning)
{
    static int counter{0};

    if (warning || left || right)
    {
        counter = (counter + 1) % 20;
    }
    else
    {
        counter = 0;
        player.stop();
    }

    QFont font("Material Icons", 50);
    painter.setFont(font);

    painter.setPen(QPen(Qt::green));

    if (warning == true && counter > 9)
    {
        // Define the battery icon character from Material Icons font
        QString left_icon = QChar(0xE5C4);

        painter.drawText(QPointF(50, 100), left_icon);

        QString right_icon = QChar(0xE5C8);

        painter.drawText(QPointF(580, 100), right_icon);

        player.play();
    }
    else if (left == true && counter > 9)
    {
        // Define the battery icon character from Material Icons font
        QString left_icon = QChar(0xE5C4); // Replace with actual Unicode if different

        painter.drawText(QPointF(50, 100), left_icon); // Adjust as needed

        player.play();
    }
    else if (right == true && counter > 9)
    {
        // Define the battery icon character from Material Icons font
        QString right_icon = QChar(0xE5C8); // Replace with actual Unicode if different

        painter.drawText(QPointF(580, 100), right_icon); // Adjust as needed

        player.play();
    }
}

void Canvas::paint_no_connection()
{
    // Set the color using RGB values
    QColor customColor(60, 0, 70);
    painter.setPen(customColor);
    painter.setBrush(customColor);

    // Draw a rectangle to block the background fonts and texts
    painter.drawRect(300, 420, 200, 150);

    // adding the text
    QFont textFont("Arial", 20); // Example font, adjust as needed
    painter.setFont(textFont);

    QString errorText = "Connection Error";                  // Convert speed to string
    QRect errorRect = QRect(200, 490, 300, 40);              // Define the rectangle for the speed text
    painter.setPen(QPen(Qt::red, 10));                       // Set pen color and width
    painter.drawText(errorRect, Qt::AlignCenter, errorText); // Draw the speed text

    QFont font2("Material Icons", 50);
    painter.setFont(font2);
    QString iconunicode = QString(QChar(0xE628));
    painter.drawText(QPointF(318, 495), iconunicode);
}
