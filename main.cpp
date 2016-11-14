#include <QCoreApplication>
#include "gcode_info.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    gcode_info *mygcode = new gcode_info();
    qDebug()<<"start"<<endl;

    mygcode->analyzer(QString(argv[1]));
    mygcode->print_info();

    exit(0);

    return a.exec();
}
