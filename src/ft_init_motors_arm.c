#include "../include/robotic_arm.h"
#include "../include/Rozum-Servo-Drives-API/c/src/usbcan_proto.h"




/**
 * @brief Activates a single Rozum servo motor.
 * Sends a command to switch the motor to the NMT (Network Management) OPERATIONAL state, 
 * which is required before the motor can execute movement commands.
 * It waits and checks up to 20 times (2 seconds total) for the state change to confirm.
 * * @param motor Pointer to the Rozum servo structure.
 */
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


/**
 * @brief Initializes the CAN interface and the three Rozum motors for the arm base.
 * Sets up the communication port, binds the default IDs (123, 124, 125) to the motor objects,
 * activates them, and configures the parameter cache to optimize continuous reading of 
 * position, current, and temperature.
 * * @param arm Pointer to the RoboArm structure holding the motor interfaces.
 * @param var_m Pointer to the variable struct to initialize starting values (zeros).
 */
void init_motors_arm(RoboArm *arm,Var_motors *var_m)
{

    // Initialize the CAN USB interface
    rr_can_interface_t *iface_0 = rr_init_interface("/dev/rozum_api");
    
    arm->iface = iface_0;
    arm->motor_1 = rr_init_servo(arm->iface,123);
    arm->motor_2 = rr_init_servo(arm->iface,124);
    arm->motor_3 = rr_init_servo(arm->iface,125);
    if(arm->motor_1 == NULL && arm->motor_2 == NULL && arm->motor_3 == NULL)
    {
        printf("unrecognized motors\n");
    }
    else
    {
        printf("Se ha reconocido los motores\n");
    
    }
    // Switch all three motors to operational mode
    activate_motor_arm(arm->motor_1);
    activate_motor_arm(arm->motor_2);
    activate_motor_arm(arm->motor_3);
    
    // Setup cache entries to speed up cyclic telemetry reading (reduces CAN bus load)
    rr_param_cache_setup_entry(arm->motor_1, APP_PARAM_POSITION, true);
    rr_param_cache_setup_entry(arm->motor_2, APP_PARAM_POSITION, true);
    rr_param_cache_setup_entry(arm->motor_3, APP_PARAM_POSITION, true);

    rr_param_cache_setup_entry(arm->motor_1, APP_PARAM_CURRENT_INPUT, true);
    rr_param_cache_setup_entry(arm->motor_2, APP_PARAM_CURRENT_INPUT, true);
    rr_param_cache_setup_entry(arm->motor_3, APP_PARAM_CURRENT_INPUT, true);

    rr_param_cache_setup_entry(arm->motor_1, APP_PARAM_TEMPERATURE_ACTUATOR, true);
    rr_param_cache_setup_entry(arm->motor_2, APP_PARAM_TEMPERATURE_ACTUATOR, true);
    rr_param_cache_setup_entry(arm->motor_3, APP_PARAM_TEMPERATURE_ACTUATOR, true);  

    // Perform an initial cache update
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);
    

    // Initialize variables values to 0
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

    // Read initial physical positions to sync variables
    rr_read_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);
}


/**
 * @brief Safely shuts down and de-initializes the Rozum arm motors, freeing memory.
 */
void finish_motors_arm(RoboArm *arm)
{
   rr_deinit_servo(&arm->motor_1);
   rr_deinit_servo(&arm->motor_2);
   rr_deinit_servo(&arm->motor_3);
}


/**
 * @brief Extracts the specific CAN hardware ID from a given motor instance.
 * @return The integer ID of the device on the CAN bus.
 */
int get_motor_id(rr_servo_t **motor)
{
    int id = 0;
    usbcan_device_t *dev = (usbcan_device_t *)(*motor)->dev;
	id = dev->id;
    return id;
}


/**
 * @brief Retrieves the CAN hardware IDs of all three Rozum arm motors into an array.
 */
void get_motors_arm_id(RoboArm *arm,int id[3])
{
    id[0] = get_motor_id(&arm->motor_1);
    id[1] = get_motor_id(&arm->motor_2);
    id[2] = get_motor_id(&arm->motor_3);
}


/**
 * @brief Changes the CAN hardware IDs for the three Rozum arm motors and saves them to flash memory.
 * Note: A power cycle might be required for the new IDs to take full effect depending on the firmware.
 */
void set_motors_arm_id(RoboArm *arm,int id[3])
{
    rr_change_id_and_save(arm->iface,&arm->motor_1,id[0]);   
    rr_change_id_and_save(arm->iface,&arm->motor_2,id[1]);
    rr_change_id_and_save(arm->iface,&arm->motor_3,id[2]);
}


/**
 * @brief Refreshes the telemetry cache and reads the current temperature of the arm motors.
 */
void read_temp_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_3);

}


/**
 * @brief Refreshes the telemetry cache and reads the electrical current load of the arm motors.
 */
void read_current_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_3);
}


/**
 * @brief Refreshes the telemetry cache and reads the absolute position of the arm motors.
 */
void read_pos_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);

}


/**
 * @brief Refreshes the telemetry cache and reads the actual velocity of the arm motors.
 */
void read_vel_arm(RoboArm *arm,Var_motors *var_m)
{
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);

    rr_read_parameter(arm->motor_1,APP_PARAM_VELOCITY,&var_m->v.motor_1);
    rr_read_parameter(arm->motor_2,APP_PARAM_VELOCITY,&var_m->v.motor_2);
    rr_read_parameter(arm->motor_3,APP_PARAM_VELOCITY,&var_m->v.motor_3);

}