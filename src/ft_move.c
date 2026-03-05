#include "../include/robotic_arm.h"


char *dir;

char *dir1;
char *dir2;
char *dir3;

rr_servo_t *motor_select(RoboArm arm,char *inf)
{
    rr_servo_t *motor;
    if(inf[0] == 'm')
    {
        if(inf[1] == '1')
        {
            if(inf[2] == 'l')
            {
                motor = arm.motor_1;
                dir = "left";
            }
            else if(inf[2] == 'r')
            {
                motor = arm.motor_1;
                dir = "right";
            }
            else
            {
                fprintf(stderr, "Error: Accion de motor 1 incorrecta.\n");
                exit(1);
            }
        }
        else if(inf[1] == '2')
        {
            if(inf[2] == 'u')
            {
                motor = arm.motor_2;
                dir = "up_2";
            }
            else if(inf[2] == 'd')
            {
                motor = arm.motor_2;
                dir = "down_2";
            }
            else
            {
                fprintf(stderr, "Error: Accion de motor 2 incorrecta.\n");
                exit(1);
            }
        }
        else if(inf[1] == '3')
        {
            if(inf[2] == 'u')
            {
                motor = arm.motor_3;
                dir = "up_3";
            }
            else if(inf[2] == 'd')
            {
                motor = arm.motor_3;
                dir = "down_3";
            }
            else
            {
                fprintf(stderr, "Error: Accion de motor 3 incorrecta.\n");
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "Error: Numero de motor  incorrecto.\n");
            exit(1); 
        }
    }
    else
    {
        fprintf(stderr, "Error: Formato de motor incorrecto.\n");
        exit(1);
    }
    return motor;
}


RoboArm motors_select(RoboArm arm, char *inf)
{
    RoboArm robot;
    
    if(inf[0] == 'm' && inf[3] == 'm' && inf[6] == 'm')
    {
        if(inf[1] == '1' && inf[4] == '2' && inf[7] == '3')
        {
            //Motor 1
            if(inf[2] == 'l')
            {
                dir1 = "left";
                robot.motor_1 = arm.motor_1;
            }
            else if(inf[2] == 'r')
            {
                dir1 = "right";
                robot.motor_1 = arm.motor_1;
            }
            else if(inf[2] == '0')
            {
                dir1 = "off_1";
                robot.motor_1 = arm.motor_1;
            }
            else
            {
                fprintf(stderr, "Error: Accion de motor 1 incorrecta.\n");
                exit(1);
            }
            
            //Motor 2
            if(inf[5] == 'u')
            {
                dir2 = "up_2";
                robot.motor_2 = arm.motor_2;
            }
            else if(inf[5] == 'd')
            {
                dir2 = "down_2";
                robot.motor_2 = arm.motor_2;
            }
            else if(inf[5] == '0')
            {
                dir2 = "off_2";
                robot.motor_2 = arm.motor_2;
            }
            else
            {
                fprintf(stderr, "Error: Accion de motor 2 incorrecta.\n");
                exit(1);
            }

            //Motor 3
            if(inf[8] == 'u')
            {
                dir3 = "up_3";
                robot.motor_3 = arm.motor_3;
            }
            else if(inf[8] == 'd')
            {
                dir3 = "down_3";
                robot.motor_3 = arm.motor_3;
            }
            else if(inf[8] == '0')
            {
                dir3 = "off_3";
                robot.motor_3 = arm.motor_3;
            }
            else
            {
                fprintf(stderr, "Error: Accion de motor 3 incorrecta.\n");
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "Error: Numero de motor incorrecto.\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Error: Formato de motor incorrecto.\n");
        exit(1);
    }
    if(robot.motor_1 == NULL || robot.motor_2 == NULL || robot.motor_3 == NULL)
    {
        fprintf(stderr, "Error: Motores no inicializados.\n");
        exit(1);
    }
    return robot;
}

int	ft_strcmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
	{
		return (0);
	}
	while ((s1[i] != '\0' && s2[i] != '\0') && s1[i] == s2[i] && i < n - 1)
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


void auto_move_motor(RoboArm arm, long time, char *inf)
{
    TimerMS subida;
    TimerMS bajada;
    rr_servo_t *motor;
    long t = time;
    float p;
    float v;
    bool terminado = false;
    bool flag = false;
    motor = motor_select(arm,inf);
    if(motor == NULL)
    {
        fprintf(stderr, "Error: Motor no inicializado.\n");
        exit(1);   
    }
    else
    {
        iniciar_timer_ms(&subida, t);
    }
    while(!terminado)
    {
        rr_param_cache_update(motor);
        rr_read_cached_parameter(motor,APP_PARAM_POSITION,&p);
        if(!timer_ms_expirado(&subida))
        {
            if(ft_strcmp(dir,"up_3",4) == 0)
            {
                if(p > -193)
                {
                    if(v > -5)
                    {
                        v-=0.5;
                    }
                }
                else
                {
                    if(v < 0)
                    {
                        v+=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir,"up_2",4) == 0)
            {
                if(p < -27)
                {
                    if(v < 5)
                    {
                        v+=0.5;
                    }
                }
                else
                {
                    if(v > 0)
                    {
                        v-=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir,"left",4) == 0)
            {
                if(p < 72)
                {
                    if(v < 5)
                    {
                        v+=0.5;
                    }
                }
                else
                {
                    if(v > 0)
                    {
                        v-=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir,"down_3",6) == 0)
            {
                if(p < -40)
                {
                    if(v < 5)
                    {
                        v+=0.5;
                    }
                }
                else
                {
                    if(v > 0)
                    {
                        v-=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir,"down_2",6) == 0)
            {
                if(p > -121)
                {
                    if(v > -5)
                    {
                        v-=0.5;
                    }
                }
                else
                {
                    if(v < 0)
                    {
                        v+=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir,"right",5) == 0)
            {
                if(p > -103)
                {
                    if(v > -5)
                    {
                        v-=0.5;
                    }
                }
                else
                {
                    if(v < 0)
                    {
                        v+=0.5;
                    }
                }
            }
        }
        else
        {
            if(!flag)
            {
                iniciar_timer_ms(&bajada, time*0.1);
                flag = true;
            }
            if(!timer_ms_expirado(&bajada))
            {
                if((ft_strcmp(dir,"up_3",4) == 0) || (ft_strcmp(dir,"down_2",6) == 0) || (ft_strcmp(dir,"right",5) == 0))
                {
                    if(v < 0)
                    {
                        v+=0.5;
                        printf("estoy bajando o estoy bajando\n");
                    }
                }
                else if((ft_strcmp(dir,"down_3",6) == 0) || (ft_strcmp(dir,"up_2",4) == 0) || (ft_strcmp(dir,"left",4) == 0))
                {
                    if(v > 0)
                    {
                        v-=0.5;
                    }
                }
            }
            else
            {
                terminado = true;
                printf("DING DONG DING DONG\n");
            }
        }
        usleep(20000);
        rr_set_velocity(motor,v);
    }
}


void auto_move_motors(RoboArm arm,Var_motors var, Tmotors times, char *inf)
{
    TimerMS subida_1;
    TimerMS bajada_1;
    TimerMS subida_2;
    TimerMS bajada_2;
    TimerMS subida_3;
    TimerMS bajada_3;
    RoboArm robot;
    long t1 = times.motor_1;
    long t2 = times.motor_2;
    long t3 = times.motor_3;
    bool terminado_total = false;
    bool terminado1 = false;
    bool terminado2 = false;
    bool terminado3 = false;
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;
    var.v.motor_1 = 0;
    var.v.motor_2 = 0;
    var.v.motor_3 = 0;


    robot = motors_select(arm,inf);
    if(robot.motor_1 == NULL || robot.motor_2 == NULL || robot.motor_3 == NULL)
    {
        fprintf(stderr, "Error: Motores no inicializados.\n");
        exit(1);
    }
    else
    {
        iniciar_timer_ms(&subida_1, t1*0.9);
        iniciar_timer_ms(&subida_2, t2*0.9);
        iniciar_timer_ms(&subida_3, t3*0.9);
    }
    while(!terminado_total)
    {
        rr_param_cache_update(robot.motor_1);
        rr_read_cached_parameter(robot.motor_1,APP_PARAM_POSITION,&var.p.motor_1);
        rr_param_cache_update(robot.motor_2);
        rr_read_cached_parameter(robot.motor_2,APP_PARAM_POSITION,&var.p.motor_2);
        rr_param_cache_update(robot.motor_3);
        rr_read_cached_parameter(robot.motor_3,APP_PARAM_POSITION,&var.p.motor_3);
        
        if(!timer_ms_expirado(&subida_1) && (ft_strcmp(dir1,"off_1",5) != 0))
        {
            if(ft_strcmp(dir1,"left",4) == 0)
            {
                if(var.p.motor_1 < 72)
                {
                    if(var.v.motor_1 < 5)
                    {
                        var.v.motor_1+=0.5;
                    }
                }
                else
                {
                    if(var.v.motor_1 > 0)
                    {
                        var.v.motor_1-=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir1,"right",5) == 0)
            {
                if(var.p.motor_1 > -103)
                {
                    if(var.v.motor_1 > -5)
                    {
                        var.v.motor_1-=0.5;
                    }
                }
                else
                {
                    if(var.v.motor_1 < 0)
                    {
                        var.v.motor_1+=0.5;
                    }
                }
            }

        }
        else
        {
            if(!flag1)
            {
                iniciar_timer_ms(&bajada_1, t1*0.1);
                flag1 = true;
            }
            if(!timer_ms_expirado(&bajada_1))
            {
                if((ft_strcmp(dir1,"right",5) == 0))
                {
                    if(var.v.motor_1 < 0)
                    {
                        var.v.motor_1+=0.5;
                    }
                }
                else if((ft_strcmp(dir1,"left",4) == 0))
                {
                    if(var.v.motor_1 > 0)
                    {
                        var.v.motor_1-=0.5;
                    }
                }
            }
            else
            {
                terminado1 = true;
            }
        }
        
        if(!timer_ms_expirado(&subida_2) && (ft_strcmp(dir2,"off_2",5) != 0))
        {
            if(ft_strcmp(dir2,"up_2",4) == 0)
            {
                if(var.p.motor_2 < -27)
                {
                    if(var.v.motor_2 < 5)
                    {
                        var.v.motor_2+=0.5;
                    }
                }
                else
                {
                    if(var.v.motor_2 > 0)
                    {
                        var.v.motor_2-=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir2,"down_2",6) == 0)
            {
                if(var.p.motor_2 > -121)
                {
                    if(var.v.motor_2 > -5)
                    {
                        var.v.motor_2-=0.5;
                    }
                }
                else
                {
                    if(var.v.motor_2 < 0)
                    {
                        var.v.motor_2+=0.5;
                    }
                }
            }
        }
        else
        {
            if(!flag2)
            {
                iniciar_timer_ms(&bajada_2, t2*0.1);
                flag2 = true;
            }
            if(!timer_ms_expirado(&bajada_2))
            {
                if((ft_strcmp(dir2,"down_2",6) == 0))
                {
                    if(var.v.motor_2 < 0)
                    {
                        var.v.motor_2+=0.5;
                    }
                }
                else if((ft_strcmp(dir2,"up_2",4) == 0))
                {
                    if(var.v.motor_2 > 0)
                    {
                        var.v.motor_2-=0.5;
                    }
                }
            }
            else
            {
                terminado2 = true;
            }
        }

        if(!timer_ms_expirado(&subida_3) && (ft_strcmp(dir3,"off_3",5) != 0))
        {
            if(ft_strcmp(dir3,"up_3",4) == 0)
            {
                if(var.p.motor_3 > -193)
                {
                    if(var.v.motor_3 > -5)
                    {
                        var.v.motor_3-=0.5;
                    }
                }
                else
                {
                    if(var.v.motor_3 < 0)
                    {
                        var.v.motor_3+=0.5;
                    }
                }
            }
            else if(ft_strcmp(dir3,"down_3",6) == 0)
            {
                if(var.p.motor_3 < -40)
                {
                    if(var.v.motor_3 < 5)
                    {
                        var.v.motor_3+=0.5;
                    }
                }
                else
                {
                    if(var.v.motor_3 > 0)
                    {
                        var.v.motor_3-=0.5;
                    }
                }
            }
        }
        else
        {
            if(!flag3)
            {
                iniciar_timer_ms(&bajada_3, t3*0.1);
                flag3 = true;
            }
            if(!timer_ms_expirado(&bajada_3))
            {
                if((ft_strcmp(dir3,"up_3",4) == 0))
                {
                    if(var.v.motor_3 < 0)
                    {
                        var.v.motor_3+=0.5;
                    }
                }
                else if((ft_strcmp(dir3,"down_3",6) == 0))
                {
                    if(var.v.motor_3 > 0)
                    {
                        var.v.motor_3-=0.5;
                    }
                }
            }
            else
            {
                terminado3 = true;
            }
        }
        if(terminado1 == true && terminado2 == true && terminado3 == true)
        {
            terminado_total = true;
        }
        usleep(20000);
        rr_set_velocity(robot.motor_1,var.v.motor_1);
        rr_set_velocity(robot.motor_2,var.v.motor_2);
        rr_set_velocity(robot.motor_3,var.v.motor_3);
    }

}