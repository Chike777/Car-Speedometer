#include "window.h"

#ifdef UART
#include "serialservice.h"
#else
#include "tcpservice.h"
#endif

#include <QApplication>

int main(int argc, char **argv)
{

    QApplication app(argc, argv);

#ifdef UART
    SerialService service;
#else
    TCPService service;
#endif

    Window window(service);

    window.show();

    return app.exec();
}
