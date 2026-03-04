#ifndef ROBOTIC_ARM_H

#define ROBOTIC_ARM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "./Rozum-Servo-Drives-API/c/include/api.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


typedef struct {
    struct timespec inicio;
    long ms_objetivo;
} TimerMS;

typedef struct
{
    rr_servo_t *motor_1;
    rr_servo_t *motor_2;
    rr_servo_t *motor_3;
}RoboArm;

typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;

}Tmotors;

typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;

}Pos_motors;


typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;

}Vel_motors;


typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;

}Temp_motors;

typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;

}Current_motors;


typedef struct 
{
    Pos_motors p;
    Vel_motors v;
    Current_motors c;
    Temp_motors t; 
}Var_motors;

void iniciar_timer_ms(TimerMS *t, long milisegundos);
bool timer_ms_expirado(TimerMS *t);
void init_motors(RoboArm *arm,Var_motors *var_m);
void manual_mode(RoboArm arm, Var_motors var_m);
void auto_move_motor(RoboArm arm, long time, char *inf);
void auto_move_motors(RoboArm arm,Var_motors var, Tmotors times, char *inf);



#endif
