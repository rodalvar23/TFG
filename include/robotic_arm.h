#ifndef ROBOTIC_ARM_H

#define ROBOTIC_ARM_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "./Rozum-Servo-Drives-API/c/include/api.h"


//dynamixel motors ids
#define ID_1 1
#define ID_2 2
#define ID_3 3
#define ID_5 5
#define ID_12 12

//Constants
#define ON 1
#define OFF 0
#define OPERATING_MODE 11
#define TORQUE_ADDRESS 64
#define VELOCITY_MODE 1
#define VELOCITY_LIMIT 48
#define POSITION_LIMIT 4
#define POSITION_MODE 3
#define PRESENT_CURRENT_ADDRESS 126
#define PRESENT_VELOCITY_ADDRESS 128
#define PRESENT_POSITION_ADDRESS 132
#define PRESENT_TEMPERATURE_ADDRESS 146
#define GOAL_VELOCITY_ADDRESS 104
#define GOAL_POSITION_ADDRESS 116



typedef struct
{
    rr_servo_t *motor_1;
    rr_servo_t *motor_2;
    rr_servo_t *motor_3;
    rr_can_interface_t *iface;
}RoboArm;

typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;
    float motor_4;
    float motor_5;
    float motor_6;
    float motor_7;
    float motor_8;
}Pos_motors;


typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;
    float motor_4;
    float motor_5;
    float motor_6;
    float motor_7;
    float motor_8;
}Vel_motors;


typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;
    float motor_4;
    float motor_5;
    float motor_6;
    float motor_7;
    float motor_8;
}Temp_motors;

typedef struct 
{   
    float motor_1;
    float motor_2;
    float motor_3;
    float motor_4;
    float motor_5;
    float motor_6;
    float motor_7;
    float motor_8;
}Current_motors;


typedef struct 
{
    Pos_motors p;
    Vel_motors v;
    Current_motors c;
    Temp_motors t; 
}Var_motors;



#ifdef __cplusplus
#include "dynamixel_ros2.h"
#include <string>
#include <iostream>
#include <condition_variable>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <mutex>
#include <vector>
#include <array>


typedef std::array <float,8> vec_motors;
typedef std::array <int,5> result;
typedef std::array  <uint8_t,5> ids_claw;

enum class tarea_dynamixel{
    SLEEPING,
    SEND_VELOCITY,
    SEND_POSITION,
    READ_TEMPERATURE,
    READ_VELOCITY,
    READ_POSITION,
    READ_CURRENT,
    POWER_OFF
};


class Manipulator
{   

    private:
    std::thread hilo_dynamixel;
    std::mutex mtx_sincronizacion;
    std::condition_variable cv_iniciar_tarea_;
    std::condition_variable cv_tarea_terminada_;
    
    // Aquí guardamos la orden actual
    tarea_dynamixel comando_actual_ = tarea_dynamixel::SLEEPING;
    bool tarea_completada_ = false;

    void gestor_tareas();

    //Variables publicas
    public:
        RoboArm arm;
        Var_motors var_m;
        dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler("/dev/u2d2_dyn"); // your dxl port name;
        dynamixel::PacketHandler *packetHandler  = dynamixel::PacketHandler::getPacketHandler(2.0); //protocol version;
        vec_motors max_pos,vel,pos;
        
        public:
        //Constructor
        Manipulator() = default;
        //Destructor
        ~Manipulator()
        {
            
        }
        //Funciones
        void init_motors(RoboArm *arm,Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler);
        void set_max_Velocity(vec_motors vel_max);
        void set_max_Position(vec_motors pos_max);
        void set_velocity_claw();
        void set_velocity_raw();
        void set_position_claw();
        void set_position_raw();
        void set_Mode(char mode);
        void getIDS(int ids[8]);
        void setIDS(int ids[8]);
        void set_torque_state(bool state);
        void get_torque_state();
        void full_manual_mode();
        void read_temperature();
        void read_velocity();
        void read_position();
        void read_current();
        void init();
        void deinit();

  };



extern "C"{
    #endif    
    
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>


    void init_motors_arm(RoboArm *arm,Var_motors *var_m);
    void finish_motors_arm(RoboArm *arm);
    void manual_mode(RoboArm arm, Var_motors var_m);
    void get_motors_arm_id(RoboArm *arm,int id[3]);
    void set_motors_arm_id(RoboArm *arm,int id[3]);
    void read_vel_arm(RoboArm *arm,Var_motors *var_m);
    void read_current_arm(RoboArm *arm,Var_motors *var_m);
    void read_temp_arm(RoboArm *arm,Var_motors *var_m);
    void read_pos_arm(RoboArm *arm,Var_motors *var_m);
    void set_velocity_arm(RoboArm *arm,Var_motors *var_m, float vel[3], float max_position[8]);
    void set_position_arm(RoboArm *arm,Var_motors *var_m,float pos[3],float max_position[8]);

#ifdef __cplusplus
}
#endif


#endif
