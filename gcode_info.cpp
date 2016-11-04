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
    double x = 0.0,y = 0.0,z = 0.0,e = 0.0,last_z = -0.1,last_e = 0.0,count_e = 0.0;
    double f = 3600;
    unsigned int t = 0;
    bool valid_z = false;
    bool abs = true;
    QStringList s_x,s_y,s_z,s_e,s_f;
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
            if (str.contains("G92 E0"))
            {
                count_e += e;
                e = 0;
            }
            else if (str.contains("G90"))
            {
                abs = true;
            }
            else if (str.contains("G91"))
            {
                abs = false;
            }
//            qDebug()<<"not skip \n";
            s_list = str.split(' ');
//            qDebug()<<s_list;
            s_x = s_list.filter("X");
            s_y = s_list.filter("Y");
            s_z = s_list.filter("Z");
            s_e = s_list.filter("E");
            s_f = s_list.filter("F");
//            qDebug()<<s_list;
//            qDebug()<<s_x<<","<<s_y<<","<<s_z<<","<<s_e;
            valid_z = false;
            if (!s_x.empty())
            {
                valid_z = true;
                x = s_x[0].midRef(1).toDouble();
                max_x = max_x > x ? max_x : x;
            }
            if (!s_y.empty())
            {
                valid_z = true;
                y = s_y[0].midRef(1).toDouble();
                max_y = max_y > y ? max_y : y;
            }
            if (!s_f.empty())
            {
                valid_z = true;
                t += (int)((e - last_e) / (f / 3600));
                last_e = e;
                f = s_f[0].midRef(1).toInt();
            }
            if (!s_e.empty())
            {
                if (abs)
                {
                    e = s_e[0].midRef(1).toDouble();
                }
                else
                {
                    e += s_e[0].midRef(1).toDouble();
                }
            }
            if (!s_z.empty())
            {
                z = s_z[0].midRef(1).toDouble();
                max_z = max_z > z ? max_z : z;
                if (valid_z && z > last_z)
                {
                    layers++;
                    qDebug()<<str;
                }
                 last_z = z;
            }
//            qDebug()<<"E";
//            qDebug()<<x<<","<<y<<","<<z<<endl;
        }
        this->layers = layers;
    }
    qDebug()<<layers;
    qDebug()<<t;
    qDebug()<<max_x<<","<<max_y<<","<<max_z<<","<<e;
    qDebug()<<"done \n";
    return 0;
}

int gcode_info::print_info()
{

}
