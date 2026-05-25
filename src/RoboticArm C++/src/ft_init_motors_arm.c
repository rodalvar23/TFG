#include "../include/robotic_arm.h"
#include "../include/Rozum-Servo-Drives-API/c/src/usbcan_proto.h"





void activate_motor_arm(rr_servo_t *motor)
{
    rr_servo_set_state_operational(motor);
 
    rr_nmt_state_t state = 0;
    for(int i = 0; i < 20; i++)
    {
        rr_sleep_ms(100);
        rr_servo_get_state(motor, &state);
        if(state == RR_NMT_OPERATIONAL)
        {
            break;
        }
    }
    if(state != RR_NMT_OPERATIONAL)
    {
        API_DEBUG("Can't switch tot operational mode\n");
        exit(1);
    }
}



void init_motors_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_can_interface_t *iface_0 = rr_init_interface("/dev/rozum_api");
    
    arm->iface = iface_0;
    arm->motor_1 = rr_init_servo(arm->iface,123);
    arm->motor_2 = rr_init_servo(arm->iface,124);
    arm->motor_3 = rr_init_servo(arm->iface,125);
    if(arm->motor_1 == NULL && arm->motor_2 == NULL && arm->motor_3 == NULL)
    {
        printf("No se ha reconocido los motores\n");
    }
    else
    {
        printf("Se ha reconocido los motores\n");
    
    }
    activate_motor_arm(arm->motor_1);
    activate_motor_arm(arm->motor_2);
    activate_motor_arm(arm->motor_3);
    
    rr_param_cache_setup_entry(arm->motor_1, APP_PARAM_POSITION, true);
    rr_param_cache_setup_entry(arm->motor_2, APP_PARAM_POSITION, true);
    rr_param_cache_setup_entry(arm->motor_3, APP_PARAM_POSITION, true);

    rr_param_cache_setup_entry(arm->motor_1, APP_PARAM_CURRENT_INPUT, true);
    rr_param_cache_setup_entry(arm->motor_2, APP_PARAM_CURRENT_INPUT, true);
    rr_param_cache_setup_entry(arm->motor_3, APP_PARAM_CURRENT_INPUT, true);

    rr_param_cache_setup_entry(arm->motor_1, APP_PARAM_TEMPERATURE_ACTUATOR, true);
    rr_param_cache_setup_entry(arm->motor_2, APP_PARAM_TEMPERATURE_ACTUATOR, true);
    rr_param_cache_setup_entry(arm->motor_3, APP_PARAM_TEMPERATURE_ACTUATOR, true);  

    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);
    
    var_m->v.motor_1 = 0;
    var_m->v.motor_2 = 0;
    var_m->v.motor_3 = 0;

    var_m->p.motor_1 = 0;
    var_m->p.motor_2 = 0;
    var_m->p.motor_3 = 0;

    var_m->t.motor_1 = 0;
    var_m->t.motor_2 = 0;
    var_m->t.motor_3 = 0;

    var_m->c.motor_1 = 0;
    var_m->c.motor_2 = 0;
    var_m->c.motor_3 = 0;


    rr_read_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);
}

void finish_motors_arm(RoboArm *arm)
{
   rr_deinit_servo(&arm->motor_1);
   rr_deinit_servo(&arm->motor_2);
   rr_deinit_servo(&arm->motor_3);
}

int get_motor_id(rr_servo_t **motor)
{
    int id = 0;
    usbcan_device_t *dev = (usbcan_device_t *)(*motor)->dev;
	id = dev->id;
    return id;
}

void get_motors_arm_id(RoboArm *arm,int id[3])
{
    id[0] = get_motor_id(&arm->motor_1);
    id[1] = get_motor_id(&arm->motor_2);
    id[2] = get_motor_id(&arm->motor_3);
}

void set_motors_arm_id(RoboArm *arm,int id[3])
{
    rr_change_id_and_save(arm->iface,&arm->motor_1,id[0]);   
    rr_change_id_and_save(arm->iface,&arm->motor_2,id[1]);
    rr_change_id_and_save(arm->iface,&arm->motor_3,id[2]);
}

void read_temp_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_3);

}

void read_current_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_3);
}

void read_pos_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);

}

void read_vel_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_VELOCITY,&var_m->v.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_VELOCITY,&var_m->v.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_VELOCITY,&var_m->v.motor_3);

}