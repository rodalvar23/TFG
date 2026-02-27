#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "./Rozum-Servo-Drives-API/c/include/api.h"


//sudo fuser -k PUERTO/tcp   en caso de liberar el puerto
//motor 3 125 Se encuentra arriba del todo
//motor 2 124 Se encuentra en el medio
//motor 1 123 Se encuentra en la base


int main()
{
    rr_can_interface_t *iface = rr_init_interface("/dev/rozum_api");
    rr_servo_t *servo = rr_init_servo(iface,125);
    if(servo == NULL)
    {
        printf("No se ha reconocido el motor\n");
    }
    else
    {
        printf("Se ha reconocido el motor\n");
    }
    	//float value;
	//! [Read parameter variable]

    rr_servo_set_state_operational(servo);
	rr_set_velocity(servo,-1);
    //rr_clear_points_all(servo);
	/*float d;
   	
    while(1)
	{
		rr_read_parameter(servo,APP_PARAM_POSITION,&d);
        for(int i= 0; i<10000;i++)
        {

        }
        printf("%f",d);
        printf("\n");
	}*/
	
    /*int status = rr_add_motion_point(servo, -150.0, 0.0, 6000);
	if(status != RET_OK)
	{
		API_DEBUG("Error in the trjectory point calculation: %d\n", status);
		return 1;
	}
	//! [Add motion point first]
	//! [Add motion point second]
	status = rr_add_motion_point(servo, -100.0, 0.0, 6000);
	if(status != RET_OK)
	{
		API_DEBUG("Error in the trjectory point calculation: %d\n", status);
		return 1;
	}
	//! [Add motion point second]
	//! [Start motion1]
	rr_start_motion(iface, 0);
	*/
	//! [Start motion1]
	//! [Sleep1]
	rr_sleep_ms(14000); // wait till the movement ends*/
    return 0;
}