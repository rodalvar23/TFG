#include <ncurses.h>
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


#define UP       119
#define DOWN     115
#define LEFT     97
#define RIGHT    100
#define FOWARD   101
#define BACKWARD 113


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

}Pos_motors;


int ymax = 16;
int xmax = 178;

int ymin = 1;
int xmin = 1;

float pos = 0;
float vel = 0;



void print_mark(const char *fill)
{
    for (int i = ymin; i < ymax; i++)
    {
        for (int j = xmin; j < xmax; j++)
        {
            if((i == ymin || i == ymax -1) || (j == xmin || j == xmax - 1))
            {
                mvprintw(i,j,"+");
            }
            else
            {
                mvprintw(i,j,"%s",fill);
            }
        }
    }   
}


void move_velocity_motor(rr_servo_t *servo)
{
    int input;
    int i = 0;
    bool press_enter = false;
    mvprintw(0,0,"Velocidad del motor\n");    /* Imprime un mesaje              */
    while(!press_enter)
    {   
        input = getch();                       /* Guarda la tecla presionada     */
        if(input == UP)
        {
            if(vel > -6)
            {
                vel -=0.4;
            }
            i = 0;
        }
        else if(input == DOWN)
        {
            if(vel < 6)
            {
                vel+=0.4;

            }
            i = 0;   
        }
        if(i > 15)
        {
            i = 0;
            if(vel > 0)
            {
                vel-= 0.1;
            }
            else if(vel < 0)
            {
                vel+=0.1;
            }
            else if(vel < 0.1 && vel > -0.1)
            {
                vel = 0;
            }
            print_mark(" ");
        }
        i++;
        rr_set_velocity(servo,vel);
        mvprintw(4,5,"%f",vel);
        if(input == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();
    }
}


void move_velocity_motors(rr_servo_t *servo_1,rr_servo_t *servo_2, rr_servo_t *servo_3)
{
    int input;
    Vel_motors vel;
    vel.motor_1 = 0;
    vel.motor_2 = 0;
    vel.motor_3 = 0;
    int i = 0;
    bool press_enter = false;
    mvprintw(0,0,"Velocidad de motores\n");    /* Imprime un mesaje              */
    while(!press_enter)
    {   
        input = getch();                       /* Guarda la tecla presionada     */
        
        //Motor 1
        if(input == RIGHT)
        {
            if( vel.motor_1> -6)
            {
                vel.motor_1 -=0.4;
            }
            i = 0;
        }
        else if(input == LEFT)
        {
            if(vel.motor_1 < 6)
            {
                vel.motor_1+=0.4;

            }
            i = 0;   
        }

        //Motor 2
        else if(input == BACKWARD)
        {
            if(vel.motor_2 < 6)
            {
                vel.motor_2+=0.4;

            }
            i = 0;
        }
        else if(input == FOWARD)
        {
            if(vel.motor_2 > -6)
            {
                vel.motor_2-=0.4;

            }
            i = 0;
        }

        //Motor 3
        if(input == UP)
        {
            if( vel.motor_3 > -6)
            {
                vel.motor_3 -=0.4;
            }
            i = 0;
        }
        else if(input == DOWN)
        {
            if(vel.motor_3 < 6)
            {
                vel.motor_3 +=0.4;

            }
            i = 0;   
        }
        if(i > 5)
        {
            i = 0;
            //Motor 1
            if(vel.motor_1 > 0)
            {
                vel.motor_1-= 0.1;
            }
            else if(vel.motor_1 < 0)
            {
                vel.motor_1+=0.1;
            }
            else if(vel.motor_1 < 0.1 && vel.motor_1 > -0.1)
            {
                vel.motor_1 = 0;
            }

            // Motor 2
            if(vel.motor_2 > 0)
            {
                vel.motor_2-= 0.1;
            }
            else if(vel.motor_2 < 0)
            {
                vel.motor_2+=0.1;
            }
            else if(vel.motor_2 < 0.1 && vel.motor_2 > -0.1)
            {
                vel.motor_2 = 0;
            }

            // Motor 3
            if(vel.motor_3 > 0)
            {
                vel.motor_3-= 0.1;
            }
            else if(vel.motor_3 < 0)
            {
                vel.motor_3+=0.1;
            }
            else if(vel.motor_3 < 0.1 && vel.motor_3 > -0.1)
            {
                vel.motor_3 = 0;
            }
            print_mark(" ");
        }
        i++;
        rr_set_velocity(servo_1,vel.motor_1);
        mvprintw(4,5,"%f",vel.motor_1);

        rr_set_velocity(servo_2,vel.motor_2);
        mvprintw(6,5,"%f",vel.motor_2);

        rr_set_velocity(servo_3,vel.motor_3);
        mvprintw(8,5,"%f",vel.motor_3);
        if(input == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();
    }
}

void move_position_motor(rr_servo_t *servo)
{
    int input;
    bool press_enter = false;
    mvprintw(0,0,"Posicion del motor\n");    /* Imprime un mesaje              */
    mvprintw(2,2,"%f",pos);
    while(!press_enter)
    {

       rr_read_parameter(servo,APP_PARAM_POSITION,&pos);
       mvprintw(2,5,"%f",pos);
        
        input = getch();                       /* Guarda la tecla presionada     */
        if(input == UP)
        {
            pos -= 2.5;
            rr_set_position(servo,pos);
        }
        else if(input == DOWN)
        {
            pos+=2.5;
            rr_set_position(servo,pos);
        }
        if(input == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();
    }
}

void move_position_motors(rr_servo_t *servo_1, rr_servo_t *servo_2, rr_servo_t *servo_3)
{
    int input;
    Pos_motors position;
    bool press_enter = false;

    position.motor_1 = 0;
    position.motor_2 = 0;
    position.motor_3 = 0;

    mvprintw(0,0,"Posiciones de los motores\n");    /* Imprime un mesaje              */
    mvprintw(2,5,"%f",position.motor_1);
    mvprintw(4,5,"%f",position.motor_2);
    mvprintw(6,5,"%f",position.motor_3);
    while(!press_enter)
    {

       rr_read_parameter(servo_1,APP_PARAM_POSITION,&position.motor_1);
       rr_read_parameter(servo_2,APP_PARAM_POSITION,&position.motor_2);
       rr_read_parameter(servo_3,APP_PARAM_POSITION,&position.motor_3);
       mvprintw(2,5,"%f",position.motor_1);
       mvprintw(4,5,"%f",position.motor_2);
       mvprintw(6,5,"%f",position.motor_3);
        
        input = getch();                       /* Guarda la tecla presionada     */
        
        //Motor 1
        if(input == LEFT)
        {
            position.motor_1 -= 2.5;
            rr_set_position(servo_1,position.motor_1);
        }
        else if(input == RIGHT)
        {
            position.motor_1+=2.5;
            rr_set_position(servo_1,position.motor_1);
        }

        //Motor 2
        else if(input == BACKWARD)
        {
            position.motor_2+=2.5;
            rr_set_position(servo_2,position.motor_2);
        }
        else if(input == FOWARD)
        {
            position.motor_2-=2.5;
            rr_set_position(servo_2,position.motor_2);
        }

        //Motor 3
        else if(input == UP)
        {
            position.motor_3-=2.5;
            rr_set_position(servo_3,position.motor_3);
        }
        else if(input == DOWN)
        {
            position.motor_3+=2.5;
            rr_set_position(servo_3,position.motor_3);
        }
        if(input == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();
    }
}



void init_interface()
{
    initscr();                          /* Inicia el modo curses          */
    //raw();                              /* Desactiva buffer de linea      */
    keypad(stdscr, TRUE);               /* Obtener la tecla presionada    */
    noecho();                           /* Elimina los mensajes repetidos */
    print_mark("#");
    nodelay(stdscr, TRUE);
}



int main()
{
    rr_can_interface_t *iface = rr_init_interface("/dev/rozum_api");
    rr_servo_t *servo_3 = rr_init_servo(iface,125);
    rr_servo_t *servo_2 = rr_init_servo(iface,124);
    rr_servo_t *servo_1 = rr_init_servo(iface,123);
    if(servo_3 == NULL && servo_2 == NULL && servo_1 == NULL)
    {
        printf("No se ha reconocido los motores\n");
    }
    else
    {
        printf("Se ha reconocido los motores\n");
    
    }
    //Activar el modo operativo de los motores
    rr_servo_set_state_operational(servo_3);
    rr_servo_set_state_operational(servo_2);
    rr_servo_set_state_operational(servo_1);
    //Iniciar interfaz grafica
    init_interface();
    move_velocity_motors(servo_1,servo_2,servo_3);
    //move_position_motors(servo_3,servo_2,servo_1);
    //move_velocity_motor(servo_2);
    //move_position_motors(servo_1,servo_2,servo_3);
    getch();                            /* Esperar entrada de usuario     */
    endwin();                           /* Terminar el modo curses        */

  return 0;
}