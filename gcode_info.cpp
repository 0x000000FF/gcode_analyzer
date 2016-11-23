#include "gcode_info.h"
#include <QDebug>

gcode_info::gcode_info()
{
    this->model_size[0] = 0;
    this->model_size[1] = 0;
    this->model_size[2] = 0;
    this->filament_length = 0.0;
    this->filament_weight = 0.0;
    this->layers = "";
    this->layer_height = 0.0;
    this->print_speed = 0;
    this->move_speed = 0;
    this->temper_ex = 0.0;
    this->temper_bed = 0.0;
    this->print_time = 0;
    this->layer_count = 0;
}

int gcode_info::analyzer(QString file)
{
    this->file_name = file;
    qDebug() << file;

    this->f_gcode.setFileName(file);
    if ( !this->f_gcode.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open file faild,source busy" ;
        return -1;
    }
    this->gcode_stream.setDevice(&this->f_gcode);

    double max_x = 0,max_y = 0,max_z = 0;
    double x = 0.0,y = 0.0,z = 0.0,e = 0.0,last_e = 0.0,count_e = 0.0,last_z = 0.0,current_e = 0.0;
    bool found_Z = false;
    double f = 3600;
    unsigned g = 0;
    unsigned int t = 0;
    bool abs = true;
    int newlayer_flag = -2;
    int layers = 0;
    int i = 0,j = 0,l =0;
    int line = 0;
    QString str = "";
    QString layerinfo = "";

    qDebug()<<"load file success";

    for(str = this->gcode_stream.readLine();!this->gcode_stream.atEnd();str = this->gcode_stream.readLine(),line++)
    {
        if (str.length() < 2)
        {
            continue;
        }
        if (str.startsWith(';'))
        {
//            qDebug()<<"skip \n";
        }

        else if (str.startsWith('G'))
        {
//            qDebug()<<"gcode lline"<<str;
            l = str.size();
            e = -10000;
            found_Z = false;
            newlayer_flag = 0;
            for (i = 0;i < l;i++)
            {
                switch (str.at(i).toLatin1()) {
                case 'G':
                    j = str.indexOf(' ',(i + 1));
                    j = j < 0 ? l : j;
//                    qDebug()<<str.mid((i + 1),(j - i -1));
                    g = str.mid((i + 1),(j - i -1)).toInt();
                    i = j;
                    break;
                case 'X':
                    newlayer_flag++;
                    j = str.indexOf(' ',(i + 1));
                    j = j < 0 ? l : j;
//                    qDebug()<<str.mid((i + 1),(j - i -1));
                    x = str.mid((i + 1),(j - i -1)).toDouble();
                    max_x = max_x > x ? max_x : x;
                    i = j;
                    break;
                case 'Y':
                    newlayer_flag++;
                    j = str.indexOf(' ',(i + 1));
                    j = j < 0 ? l : j;
//                    qDebug()<<str.mid((i + 1),(j - i -1));
                    y = str.mid((i + 1),(j - i - 1)).toDouble();
                    max_y = max_y > y ? max_y : y;
                    i = j;
                    break;
                case 'Z':
                    found_Z = true;
                    j = str.indexOf(' ',(i + 1));
                    j = j < 0 ? l : j;
//                    qDebug()<<str.mid((i + 1),(j - i -1));
                    z = str.mid((i + 1),(j - i - 1)).toDouble();
                    max_z = max_z > z ? max_z : z;
                    i = j;
//                    qDebug()<<last_z<<z;
                    break;
                case 'E':
                    j = str.indexOf(' ',(i + 1));
                    j = j < 0 ? l : j;
//                    qDebug()<<str.mid((i + 1),(j - i -1));
                    e = str.mid((i + 1),(j - i - 1)).toDouble();
                    i = j;
                    break;
                case 'F':
                    newlayer_flag++;
                    j = str.indexOf(' ',(i + 1));
                    j = j < 0 ? l : j;
//                    qDebug()<<str.mid((i + 1),(j - i -1));
                    f = str.mid((i + 1),(j - i - 1)).toDouble();
                    t += (count_e + last_e - current_e) / (f / 3600);
                    current_e = count_e + last_e;
                    i = j;
                    break;

                case ';':
                    i = l;
                default:
                    break;
                }
            }
            switch (g) {
            case 90:
                abs = true;
                break;
            case 91:
                abs = false;
                break;
            case 92:
                count_e += last_e;
                last_e = 0.0;
                break;
            default:
                break;
            }
            if (e != -10000)
            {
                last_e = abs ? e : (last_e + e);
            }
            if (found_Z && newlayer_flag > 0)
            {
//                qDebug()<<"--"<<last_z<<z;
                if((z > last_z) && (z - last_z) < 0.5)
                {
//                    qDebug()<<"++"<<last_z<<z;
                    layers ++;
                    last_z = z;
                    layerinfo.append(QString::number(line,10)+",");
                }
            }
        }
        else if (str.startsWith('M'))
        {
            int sp,np;
            if ( this->temper_ex == 0 && (str.contains("M104") || str.contains("M109")) )
            {
                sp = str.indexOf('S',0);
                np = str.indexOf(QRegularExpression("([0-9][^0-9])"), sp);
                this->temper_ex = str.mid(sp+1,np).toDouble();
            }
            else if ( this->temper_ex == 0 &&  (str.contains("M140") || str.contains("M190")) )
            {
                sp = str.indexOf('S',0);
                np = str.indexOf(QRegularExpression("([0-9][^0-9])"), sp);
                this->temper_bed = str.mid(sp+1,np).toDouble();
            }
        }
    }
    count_e += last_e;

    this->model_size[0] = max_x;//mm
    this->model_size[1] = max_y;//mm
    this->model_size[2] = max_z;//mm
    this->filament_length = count_e;// mm
    this->filament_weight = count_e * 3 / 1000;// g
    this->layers = layers;
    this->print_time = t; // s
    this->layer_count = layers;
    this->layers = layerinfo;

    qDebug()<<"done \n";
    return 0;
}

QString md5sum(QString filePath)
{
    QFile theFile(filePath);
    theFile.open(QIODevice::ReadOnly);
    QByteArray ba = QCryptographicHash::hash(theFile.readAll(),QCryptographicHash::Md5);
    theFile.close();
    return QString(ba.toHex().constData());
    qDebug() << ba.toHex().constData();
}

int gcode_info::print_info()
{
    QString md5 = md5sum(this->file_name);
    QString out_file = this->file_name.mid(0,this->file_name.lastIndexOf('.'))+".info";
    out_file = out_file.toLocal8Bit();
    qDebug()<<out_file;
    QFile out_gcode(out_file);
    if (out_gcode.exists())
    {
        out_gcode.remove();
    }
    out_gcode.setFileName(out_file);
    if (!out_gcode.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return -1;
    }
    QTextStream out_stream(&out_gcode);

    out_stream<<"[mostfunPro]"     <<endl;
    out_stream<<"md5sum="          <<md5 <<endl;
    out_stream<<"printing_state=normal" <<endl;
//    out_stream<<"start_line=0" <<endl;
//    out_stream<<"x_pos=0" <<endl;
//    out_stream<<"y_pos=0" <<endl;
//    out_stream<<"z_pos=0" <<endl;
//    out_stream<<"e_pos=0" <<endl;
//    out_stream<<"f_pos=0" <<endl;
    out_stream<<"max_x="           <<QString::number(this->model_size[0])<<endl;
    out_stream<<"max_y="           <<QString::number(this->model_size[1])<<endl;
    out_stream<<"max_z="           <<QString::number(this->model_size[2])<<endl;
    out_stream<<"filament_length=" <<this->filament_length  <<endl;
    out_stream<<"filament_weight=" <<this->filament_weight  <<endl;
    out_stream<<"layer_count="     <<this->layer_count      <<endl;
    out_stream<<"current_layer=0"                           <<endl;
    out_stream<<"temper_ex="       <<this->temper_ex        <<endl;
    out_stream<<"temper_bed="      <<this->temper_bed       <<endl;
    out_stream<<"print_time="      <<this->print_time       <<endl;
    out_stream<<"layers="          <<this->layers           <<endl;
    out_gcode.close();

//    QProcess *p = new QProcess(this);
//    p->start("md5sum "+ out_file + " | cut -d ' ' -f 1 >> " + out_file);

    qDebug()<<this;
}
