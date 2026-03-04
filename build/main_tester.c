#include "../include/robotic_arm.h"

RoboArm arm;
Var_motors var_m;


int main()
{
    Tmotors times;
    times.motor_1 = 8000;
    times.motor_2 = 8000;
    times.motor_3 = 8000;
    init_motors(&arm,&var_m);
    auto_move_motors(arm,var_m,times,"m1lm2um3u");
    auto_move_motors(arm,var_m,times,"m1rm2dm3d");
    return 0;
}