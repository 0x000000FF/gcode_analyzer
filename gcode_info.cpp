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
    double max_x = 0,max_y = 0,max_z = 0;
    double x = 0.0,y = 0.0,z = 0.0,e = 0.0,f = 0.0;
    QStringList s_x,s_y,s_z,s_e;
    int layers = 0;
    QStringList s_list;
    QString str = "";
    this->f_gcode.setFileName(file);
    if ( !this->f_gcode.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open file faild,source busy" ;
        return -1;
    }
    qDebug()<<"load file success";
    this->gcode_stream.setDevice(&this->f_gcode);
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
//            qDebug()<<"not skip \n";
            s_list = str.split(' ');
//            qDebug()<<s_list;
            s_x = s_list.filter("X");
            s_y = s_list.filter("Y");
            s_z = s_list.filter("Z");
            s_e = s_list.filter("E");
//            qDebug()<<s_list;
//            qDebug()<<s_x<<","<<s_y<<","<<s_z<<","<<s_e;
            if (!s_x.empty())
            {
                x = s_x[0].midRef(1).toDouble();
                max_x = max_x > x ? max_x : x;
            }
//            qDebug()<<"X";
            if (!s_y.empty())
            {
                y = s_y[0].midRef(1).toDouble();
                max_y = max_y > y ? max_y : y;
            }
//            qDebug()<<"Y";
            if (!s_z.empty())
            {
                z = s_z[0].midRef(1).toDouble();
                max_z = max_z > z ? max_z : z;
                layers++;
            }
//            qDebug()<<"Z";
            if (!s_e.empty())
            {
                e += s_e[0].midRef(1).toDouble();
            }
//            qDebug()<<"E";
//            qDebug()<<x<<","<<y<<","<<z<<endl;
        }
        this->layers = layers;
    }
    qDebug()<<layers;
    qDebug()<<max_x<<","<<max_y<<","<<max_z<<","<<e;
    qDebug()<<"done \n";
    return 0;
}

int gcode_info::print_info()
{

}
