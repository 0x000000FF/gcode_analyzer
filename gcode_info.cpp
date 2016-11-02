#include "gcode_info.h"


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
