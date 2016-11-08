#ifndef GCODE_INFO_H
#define GCODE_INFO_H
#include <QString>
#include <QtCore>

class gcode_info
{
public:
    gcode_info();
private:
    QString file_name;
    QFile f_gcode;
    QTextStream gcode_stream;

    double model_size[3];
    double filament_length;
    double filament_weight;
    int layer_count;
    double layer_height;
    int print_speed;
    int move_speed;
    double temper_ex;
    double temper_bed;
    int print_time;
    QString layers;

public:
    int analyzer(QString file);
    int print_info();

};

#endif // GCODE_INFO_H
