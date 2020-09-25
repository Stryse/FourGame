#include "fourgamewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FourGameWindow w;
    w.show();
    return a.exec();
}
