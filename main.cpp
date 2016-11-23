#include <QCoreApplication>
#include "gcode_info.h"
#include <QDebug>
//#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    gcode_info *mygcode = new gcode_info();
    qDebug()<<"start"<<endl;
    QString name = QFile::decodeName(argv[1]);
//    mygcode->analyzer(QString::fromLocal8Bit(argv[1]));
    mygcode->analyzer(name);
    mygcode->print_info();

    exit(0);

    return a.exec();
}
