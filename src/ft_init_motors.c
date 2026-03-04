#include "../include/robotic_arm.h"

void activate_motors(rr_servo_t *motor)
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



void init_motors(RoboArm *arm,Var_motors *var_m)
{
    rr_can_interface_t *iface = rr_init_interface("/dev/rozum_api");
    arm->motor_1 = rr_init_servo(iface,123);
    arm->motor_2 = rr_init_servo(iface,124);
    arm->motor_3 = rr_init_servo(iface,125);
    if(arm->motor_1 == NULL && arm->motor_2 == NULL && arm->motor_3 == NULL)
    {
        printf("No se ha reconocido los motores\n");
    }
    else
    {
        printf("Se ha reconocido los motores\n");
    
    }
    activate_motors(arm->motor_1);
    activate_motors(arm->motor_2);
    activate_motors(arm->motor_3);
    
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

    rr_read_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);
}