#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define UP    259
#define DOWN  258
#define LEFT  260
#define RIGHT 261


int x = 10;
int y = 10;

int ymax = 16;
int xmax = 178;

int ymin = 1;
int xmin = 1;


int value = 0;
float pos = 0;
double vel = 0;



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



void modify_value()
{
    int input;
    int i = 0;
    bool press_enter = false;
    mvprintw(0,0,"Incrementa o decrementa el valor de este numero\n");    /* Imprime un mesaje              */
    mvprintw(2,2,"%d",value);
    while(!press_enter)
    {
        input = getch();                       /* Guarda la tecla presionada     */
        if(input == UP)
        {
            value++;
            i = 0;
        }
        else if(input == DOWN)
        {
            value--;
            i = 0;
        }
        if(i > 30000)
        {
            i = 0;
            if(value > 0)
            {
                value-= 0.25;
            }
            else if(value < 0)
            {
                value+=0.25;
            }
            print_mark(" ");
        }
        i++;
        mvprintw(2,2,"%d",value);
        if(input == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();                          /* Imprimirlo en la pantalla real */
    }
}


void update_screen()
{
    print_mark(" ");
    refresh();
}

void move_keys(int in)
{
    if(in == RIGHT)
    {
        if(x < xmax - 2)
        {
            x++;
        }
    }
    else if(in == LEFT)
    {
        if(x > xmin + 1)
        {
            x--;
        }
    }
    else if(in == UP)
    {
        if(y > ymin + 1)
        {
            y--;
        }
    }
    else if(in == DOWN)
    {
        if(y < ymax - 2)
        {
            y++;
        }
    }
}


void print_ball()
{
    int input;
    int ball = (int)'o';
    bool press_enter = false;
    mvprintw(0,0,"Mueve la bola con las teclas direccionales\n");    /* Imprime un mesaje              */
    mvprintw(x,y,"%c",ball);
    while(!press_enter)
    {
        input = getch();                       /* Guarda la tecla presionada     */
        mvprintw(y,x," ");
        move_keys(input);
        mvprintw(y,x,"%c",ball);
        if(input == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();                          /* Imprimirlo en la pantalla real */
    }
}


void print_keys()
{
    int ch;
    bool press_enter = false;
    mvprintw(0,0,"Pulse cualquier tecla\n");    /* Imprime un mesaje              */
    while(!press_enter)
    {
        ch = getch();                       /* Guarda la tecla presionada     */
        attron(A_BOLD);
        mvprintw(2,2,"%d",ch);
        attroff(A_BOLD);
        if(ch == 10)
        {
            clear();
            printw("Pulse cualquier tecla para salir\n");
            press_enter = true;
        }
        refresh();                          /* Imprimirlo en la pantalla real */
    }
}


void init_interface()
{
    initscr();                          /* Inicia el modo curses          */
    //raw();                              /* Desactiva buffer de linea      */
    keypad(stdscr, TRUE);               /* Obtener la tecla presionada    */
    noecho();                           /* Elimina los mensajes repetidos */
    print_mark(" ");
    //nodelay(stdscr, TRUE);
}



int main()
{
    
    init_interface();
    print_keys();
    getch();                            /* Esperar entrada de usuario     */
    endwin();                           /* Terminar el modo curses        */

  return 0;
}