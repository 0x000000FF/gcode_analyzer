#ifndef GCODE_INFO_H
#define GCODE_INFO_H
#include <QString>

class gcode_info
{
public:
    gcode_info();
private:
    double model_size[3];
    double filament_length;
    double filament_weight;
    int layers;
    double layer_height;
    int print_speed;
    int move_speed;
    double temper_ex;
    double temper_bed;
    int print_time;

public:
    int analyzer(QString file);
    int print_info();

};

#endif // GCODE_INFO_H
