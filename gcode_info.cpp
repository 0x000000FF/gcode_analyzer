#include "gcode_info.h"
#include <QDebug>

gcode_info::gcode_info()
{
    this->model_size[0] = 0;
    this->model_size[1] = 0;
    this->model_size[2] = 0;
    this->filament_length = 0.0;
    this->filament_weight = 0.0;
    this->layers = 0;
    this->layer_height = 0.0;
    this->print_speed = 0;
    this->move_speed = 0;
    this->temper_ex = 0.0;
    this->temper_bed = 0.0;
    this->print_time = 0;
}

int gcode_info::analyzer(QString file)
{
    QFile out_gcode(file+"~");
    if (!out_gcode.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return -1;
    }
    this->f_gcode.setFileName(file);
    if ( !this->f_gcode.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open file faild,source busy" ;
        return -1;
    }
    this->gcode_stream.setDevice(&this->f_gcode);
    QTextStream out_stream(&out_gcode);

    double max_x = 0,max_y = 0,max_z = 0;
    double x = 0.0,y = 0.0,z = 0.0,e = 0.0,last_z = -0.1,last_e = 0.0,count_e = 0.0;
    double f = 3600;
    unsigned g = 0;
    unsigned int t = 0;
    bool valid_z = false;
    bool abs = true;
    int layers = 0;
    int i = 0,j = 0;
    QString str = "";
    qDebug()<<"load file success";

    for(str = this->gcode_stream.readLine();!this->gcode_stream.atEnd();str = this->gcode_stream.readLine())
    {
        if (str.length() < 2)
        {
            continue;
        }
        if (str.startsWith(';'))
        {
//            qDebug()<<"skip \n";
        }
        else
        {
            qDebug()<<"gcode lline"<<str;
            for (i = 0;i < str.size();i++)
            {
                switch (str.at(i).toLatin1()) {
                case 'G':
                    j = str.indexOf(' ',(i + 1));
                    qDebug()<<str.mid((i + 1),(j - i -1));
                    g = str.mid((i + 1),(j - i -1)).toInt();
                    i = j;
                    break;
                case 'X':
                    j = str.indexOf(' ',(i + 1));
                    qDebug()<<str.mid((i + 1),(j - i -1));
                    x = str.mid((i + 1),(j - i -1)).toDouble();
                    i = j;
                    break;
                case 'Y':
                    j = str.indexOf(' ',(i + 1));
                    qDebug()<<str.mid((i + 1),(j - i -1));
                    y = str.mid((i + 1),(j - i - 1)).toDouble();
                    i = j;
                    break;
                case 'Z':
                    j = str.indexOf(' ',(i + 1));
                    qDebug()<<str.mid((i + 1),(j - i -1));
                    z = str.mid((i + 1),(j - i - 1)).toDouble();
                    i = j;
                    break;
                case 'E':
                    j = str.indexOf(' ',(i + 1));
                    qDebug()<<str.mid((i + 1),(j - i -1));
                    e = str.mid((i + 1),(j - i - 1)).toDouble();
                    i = j;
                    break;
                case 'F':
                    j = str.indexOf(' ',(i + 1));
                    qDebug()<<str.mid((i + 1),(j - i -1));
                    f = str.mid((i + 1),(j - i - 1)).toDouble();
                    i = j;
                    break;

                case ';':
                    i = str.size();
                default:
                    break;
                }
            }
            qDebug()<<x<<","<<y<<","<<z<<","<<e<<","<<f;
        }
        this->layers = layers;
    }
    out_gcode.close();
    qDebug()<<layers;
    qDebug()<<t;
    qDebug()<<"done \n";
    return 0;
}

int gcode_info::print_info()
{

}
