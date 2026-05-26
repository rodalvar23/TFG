#include "../include/robotic_arm.h"


//Variables Gráficas y del mando/teclado

SDL_Window* ventana;
SDL_Renderer* renderizador;
TTF_Font* fuente;
SDL_Event evento;
SDL_Color colorTexto = {255, 255, 255, 255};

std::atomic<bool> Running(true);
int contador = 0; // Nuestra variable a mostrar 
int espacioPresionado = 0; // Variable para evitar que el contador sume miles de veces por segundo si mantenemos presionado
const Uint8 *estadoTeclado;
char textoAMostrar[2000];

int filas = 2;
int columnas = 2;
int anchoCelda = 580;
int altoCelda = 70;
int xInicial = 10;  // Margen izquierdo
int yInicial = 80; // Margen superior
int yActual;
int xActual;

bool pausa = false;
SDL_GameController* mando = nullptr;

//Variables del manipulador

double vel_max = 5;
double acc_i = 0.5;
double acc_f = 0.5;

bool flag1 = false;
bool flag2 = false;

bool on_1_1 = false;
bool on_1_2 = false;
bool on_2_1 = false;
bool on_2_2 = false;
bool on_3_1 = false;
bool on_3_2 = false;
bool on_5_1 = false;
bool on_5_2 = false;
bool on_12_1 = false;
bool on_12_2 = false;



float target_vel_1 = 0;
uint32_t read_vel_1 = 0;
float target_vel_2 = 0;
uint32_t read_vel_2 = 0;
float target_vel_3 = 0;
uint32_t read_vel_3 = 0;
float target_vel_5 = 0;
uint32_t read_vel_5 = 0;
float target_vel_12 = 0;
uint32_t read_vel_12 = 0;

uint32_t read_pos_5 = 0;
uint32_t read_pos_3 = 0;
uint32_t rpos_2;
uint32_t read_pos_1 = 0;
uint32_t rpos_12 ;
uint8_t *idss[8];

uint8_t param_goal_velocity1[4];
uint8_t param_goal_velocity3[4];
uint8_t param_goal_velocity5[4];
uint8_t param_goal_velocity2[4];
uint8_t param_goal_velocity12[4];



std::vector<int> mov(8,0);


int init_win()
{
    //Esta función se encarga de inicializar la ventana donde se mostraran toda la información del manipulador

    // 1. Inicializar SDL, TTF y Mando
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    // Buscar si hay algún mando conectado al arrancar
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            mando = SDL_GameControllerOpen(i);
            if (mando) {
                std::cout << "Mando conectado: " << SDL_GameControllerName(mando) << std::endl;
                break; // Usamos el primer mando que encontremos
            }
        }
    }

    //Inicializar el teclado
    estadoTeclado = SDL_GetKeyboardState(NULL);
    
    // Iniciar el subsistema de fuentes
    if (TTF_Init() == -1) {
        printf("Error TTF: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    ventana = SDL_CreateWindow("Control del Manipulador Robótico", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1000, 0);
    renderizador = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    // 2. Cargar la fuente de Ubuntu (Tamaño 28)
    fuente = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 28);
    if (!fuente) {
        printf("Error al cargar la fuente: %s\n", TTF_GetError());
        return 1;
    }
    return 0;
}

void finish_win()
{
    //Esta función se encarga de cerrar la ventana y de liberar la memoria que ocupa

    //Liberamos memoria del sistema de fuentes y de la ventana
    TTF_CloseFont(fuente);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    
    // Liberamos memoria del mando
    if (mando) {
        SDL_GameControllerClose(mando);
    }
    // Terminamos de cerrar el proceso
    SDL_Quit();
}

void print_table()
{
    //Esta función imprime una plantilla de tabla en la ventana

    // 1. Configuración de la tabla
        // --- ZONA DE DIBUJO ---
        
        // A. Limpiar el fondo (Negro)
        SDL_SetRenderDrawColor(renderizador, 10, 0, 153, 255); 
        SDL_RenderClear(renderizador);
        
        // 2. Elegir el color de las líneas (Blanco: 255, 255, 255)
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);

        // 3. Dibujar líneas horizontales
        // Nota: Usamos <= filas para cerrar la tabla por abajo
        for (int i = 0; i <= filas; i++) {
            if(i < filas)
            {
                yActual = yInicial + (i * altoCelda);
            }
            else if(i == filas)
            {
                yActual = yInicial + (i * (altoCelda+150));
            }
            SDL_RenderDrawLine(renderizador, 
                               xInicial, yActual, // Punto de inicio (X, Y)
                               xInicial + (columnas * anchoCelda), yActual); // Punto final (X, Y)
        }

        // 4. Dibujar líneas verticales
        // Nota: Usamos <= columnas para cerrar la tabla por la derecha
        for (int j = 0; j <= columnas; j++) {
            xActual = xInicial + (j * anchoCelda);
            SDL_RenderDrawLine(renderizador, 
                               xActual, yInicial, // Punto de inicio (X, Y)
                               xActual, yInicial + (filas * (altoCelda+330))); // Punto final (X, Y)
        }
        
        
        for (int i = 0; i <= filas; i++) {
            if(i < filas)
            {
                yActual = yInicial+440 + (i * altoCelda);
            }
            else if(i == filas)
            {
                yActual = yInicial+380 + (i * (altoCelda+140));
            }
            SDL_RenderDrawLine(renderizador, 
                xInicial, yActual, // Punto de inicio (X, Y)
                xInicial + (columnas * anchoCelda), yActual); // Punto final (X, Y)
            }   
        }
        
void print_variables(RoboArm *arm, Var_motors *var_m)
        {
            //Esta función imprime en la ventana toda la información de los motores del manipulador
            // A. Imprimimos la plantilla donde iran albergados los datos (Tabla)
            print_table();
            // B. Preparamos el texto
            
            
            snprintf(textoAMostrar, sizeof(textoAMostrar),
            "\n"
            "                                     ROBOTIC MANIPULATOR INFORMATION\n\n" 
            "   Position Variables:                                      Velocity Variables:\n\n" 
            "   Position (DEG) motor 1:  %f          Velocity (DEG/s) motor 1: %f\n"
            "   Position (DEG) motor 2:  %f        Velocity (DEG/s) motor 2: %f\n"
            "   Position (DEG) motor 3:  %f        Velocity (DEG/s) motor 3: %f\n\n"
            "   Position (DEG) motor 4:  %f         Velocity (DEG/s) motor 4: %d\n"
            "   Position (DEG) motor 5:  %f         Velocity (DEG/s) motor 6: %d\n"
            "   Position (DEG) motor 6:  %f         Velocity (DEG/s) motor 6: %d\n"
            "   Position (DEG) motor 7:  %f       Velocity (DEG/s) motor 7: %d\n"
            "   Position (DEG) motor 8:  %f          Velocity (DEG/s) motor 8: %d\n\n\n"
            "   Temperature Variables:                              Current Variables:\n\n\n" 
            "   Temperature (C) motor 1:  %f        Current (A) motor 1: %f\n"
            "   Temperature (C) motor 2:  %f        Current (A) motor 2: %f\n"
            "   Temperature (C) motor 3:  %f        Current (A) motor 3: %f\n\n"
            "   Motors Velocity: %f\n"
            "   ACC_I: %f\n"
            "   ACC_F: %f\n"
            ,var_m->p.motor_1,var_m->v.motor_1,var_m->p.motor_2,var_m->v.motor_2,var_m->p.motor_3,var_m->v.motor_3,read_pos_3*0.087891,
            read_vel_3,(int32_t)read_pos_1*0.087891,read_vel_1,(int32_t)read_pos_5*0.087891,read_vel_5,(int32_t)rpos_12*0.087891,read_vel_12,(int32_t)rpos_2*0.087891,read_vel_2,
            var_m->t.motor_1,var_m->c.motor_1,var_m->t.motor_2,var_m->c.motor_2,var_m->t.motor_3,var_m->c.motor_3,vel_max,acc_i,acc_f);

        // C. Crear una imagen (Surface) con el texto
        SDL_Surface* surfaceTexto = TTF_RenderText_Blended_Wrapped(fuente, textoAMostrar, colorTexto,0);
        
        // D. Convertir la Surface en una Textura (el formato que usa la tarjeta gráfica)
        SDL_Texture* texturaTexto = SDL_CreateTextureFromSurface(renderizador, surfaceTexto);

        // E. Definir dónde y de qué tamaño se dibujará el texto
        SDL_Rect rectanguloTexto;
        rectanguloTexto.x = 0; // 50 píxeles desde la izquierda
        rectanguloTexto.y = 0; // 50 píxeles desde arriba
        rectanguloTexto.w = surfaceTexto->w; // Ancho original del texto
        rectanguloTexto.h = surfaceTexto->h; // Alto original del texto

        // F. Pegar la textura en el renderizador
        SDL_RenderCopy(renderizador, texturaTexto, NULL, &rectanguloTexto);

        // IMPORTANTE: Liberar la memoria en cada vuelta del bucle para que el PC no colapse
        SDL_FreeSurface(surfaceTexto);
        SDL_DestroyTexture(texturaTexto);

        // G. Mostrar en pantalla
        SDL_RenderPresent(renderizador);
}

void init_mov()
{
    //Esta función se asegura que todos los motores esten en un estado de reposo

    for (int i = 0; i < 8; i++)
    {
        mov[i] = 2;
    }
}

void stop_vel_motors(float &vel, double p = 1)
{
    //Esta función se encarga de llevar a cero una cierta velocidad para lograr que un motor determinado se pare

    if(vel > 0)
    {
        vel-= p;
    }
    else if(vel < 0)
    {
        vel+=p;
    }
}

void read_keyboard()
{
    //Esta funcion lee las entradas del teclado para que dependiendo de las teclas pulsados se ejecute una acción determinada

    //Limpia el teclado
    estadoTeclado = SDL_GetKeyboardState(NULL);
    //Motor 1
    if(estadoTeclado[SDL_SCANCODE_D])
    {
        mov[0] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_A] && mov[0] == 1)
        {
            mov[0] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_A])
    {
        mov[0] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_D] && mov[0] == 3)
        {
            mov[0] = 4;
        }
    }

    //Motor 2
    if(estadoTeclado[SDL_SCANCODE_E])
    {
        mov[1] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_Q] && mov[1] == 1)
        {
            mov[1] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_Q])
    {
        mov[1] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_E] && mov[1] == 3)
        {
            mov[1] = 4;
        }
    }

    //Motor 3
    if(estadoTeclado[SDL_SCANCODE_W])
    {
        mov[2] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_S] && mov[2] == 1)
        {
            mov[2] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_S])
    {
        mov[2] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_W] && mov[2] == 3)
        {
            mov[2] = 4;
        }
    }

    //Motor 4
    if(estadoTeclado[SDL_SCANCODE_J])
    {
        mov[3] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_L] && mov[3] == 1)
        {
            mov[3] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_L])
    {
        mov[3] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_J] && mov[3] == 3)
        {
            mov[3] = 4;
        }
    }

    //Motor 5
    if(estadoTeclado[SDL_SCANCODE_I])
    {
        mov[4] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_K] && mov[4] == 1)
        {
            mov[4] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_K])
    {
        mov[4] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_I] && mov[4] == 3)
        {
            mov[4] = 4;
        }
    }

    //Motor 6
    if(estadoTeclado[SDL_SCANCODE_Y])
    {
        mov[5] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_H] && mov[5] == 1)
        {
            mov[5] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_H])
    {
        mov[5] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_Y] && mov[5] == 3)
        {
            mov[5] = 4;
        }
    }
    //Motor 7 y 8
    if(estadoTeclado[SDL_SCANCODE_U])
    {
        mov[6] = 1;
        mov[7] = 1;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_O] && (mov[6] == 1 && mov[7] == 1))
        {
            mov[6] = 2;
            mov[7] = 2;
        }
    }
    if(estadoTeclado[SDL_SCANCODE_O])
    {
        mov[6] = 3;
        mov[7] = 3;
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_U] && (mov[6] == 3 && mov[7] == 3))
        {
            mov[6] = 4;
            mov[7] = 4;
        }
    }
}

void move_claw_motors(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
    /*Esta función se encarga de asignar la velocidad correspondiente para luego ser usada
      en la actuación de los motores dynamixel*/

    if(!estadoTeclado[SDL_SCANCODE_ESCAPE])
    {
        //id 1
        if(mov[4] == 1) //if(estadoTeclado[SDL_SCANCODE_I])
        {
            if(((int32_t)read_pos_1*0.087891) < 300)
            {
                if(target_vel_1 < 40)
                {
                    target_vel_1+=5;
                    on_1_1 = true;
                }
                else if(target_vel_1 == 40)
                {
                    on_1_1 = false;
                }
            }
            else
            {
                on_1_1 = true;
                stop_vel_motors(target_vel_1,10);
            }
        }
        else if(mov[4] == 2) //else if(!estadoTeclado[SDL_SCANCODE_K])
        {
            if(target_vel_1 == 0)
            {
                on_1_1 = false;
            }
            else
            {
                on_1_1 = true;
                stop_vel_motors(target_vel_1,10);
            }
        }
        if(mov[4] == 3) //if(estadoTeclado[SDL_SCANCODE_K])
        {
            if(((int32_t)read_pos_1*0.087891) > 150)
            {
                if(target_vel_1 > -40)
                {
                    target_vel_1-=5;
                    on_1_2 = true;
                }
                else if(target_vel_1 == -40)
                {
                    on_1_2 = false;
                }
            }
            else
            {
                on_1_2 = true;
                stop_vel_motors(target_vel_1,10);
            }
        }
        else if(mov[4] == 4) //else if(!estadoTeclado[SDL_SCANCODE_I])
        {
            if(target_vel_1 == 0)
            {
                on_1_2 = false;
            }
            else
            {
                on_1_2 = true;
                stop_vel_motors(target_vel_1,10);
            }
        }
        //id 3
        if(mov[3] == 1) //if(estadoTeclado[SDL_SCANCODE_J])
        {
            if(((int32_t)read_pos_3*0.087891) < 300)
            {
                if(target_vel_3 < 40)
                {
                    target_vel_3+=5;
                    on_3_1 = true;  
                }
                else if(target_vel_3 == 40)
                {
                    on_3_1 = false;  
                }
            }
            else
            {
                on_3_1 = true;
                stop_vel_motors(target_vel_3,10);
            }
        }
        else if(mov[3] == 2) //else if(!estadoTeclado[SDL_SCANCODE_L])
        {
            if(target_vel_3 == 0)
            {
                on_3_1 = false;
            }
            else
            {
                stop_vel_motors(target_vel_3,10);
                on_3_1 = true;
            }
        }
        if(mov[3] == 3) //if(estadoTeclado[SDL_SCANCODE_L])
        {
            if(((int32_t)read_pos_3*0.087891) > -40)
            {
                if(target_vel_3 > -40)
                {
                    target_vel_3-=5;
                    on_3_2 = true;
                }
                else if(target_vel_3 == -40)
                {
                    on_3_2 = false;  
                }
            }
            else
            {
                on_3_2 = true;
                stop_vel_motors(target_vel_3,10);
            }
        }
        else if(mov[3] == 4) //else if(!estadoTeclado[SDL_SCANCODE_J])
        {
            if(target_vel_3 == 0)
            {
                on_3_2 = false;  
            }
            else
            {
                stop_vel_motors(target_vel_3,10);
                on_3_2 = true;
            }
        }

        //id 5
        if(mov[5] == 1) //if(estadoTeclado[SDL_SCANCODE_Y])
        {
            if(((int32_t)read_pos_5*0.087891) < 300)
            {
                if(target_vel_5 < 40)
                {
                    target_vel_5+=5;
                    on_5_1 = true;  
                }
                else if(target_vel_5 == 40)
                {
                    on_5_1 = false;  
                }
            }
            else
            {
                on_5_1 = true;
                stop_vel_motors(target_vel_5,10);
            }
        }
        else if(mov[5] == 2) //else if(!estadoTeclado[SDL_SCANCODE_H])
        {
            if(target_vel_5 == 0)
            {
                on_5_1 = false;  
            }
            else
            {
                on_5_1 = true;
                stop_vel_motors(target_vel_5,10);
            }
        }
        if(mov[5] == 3) //if(estadoTeclado[SDL_SCANCODE_H])
        {
            if(((int32_t)read_pos_5*0.087891) > -230)
            {
                if(target_vel_5 > -40)
                {
                    target_vel_5-=5;
                    on_5_2 = true;
                }
                else if(target_vel_5 == -40)
                {
                    on_5_2 = false;  
                }
            }
            else
            {
                on_5_2 = true;
                stop_vel_motors(target_vel_5,10);
            }
        }
        else if(mov[5] == 4) //else if(!estadoTeclado[SDL_SCANCODE_Y])
        {
            if(target_vel_5 == 0)
            {
                on_5_2 = false;  
            }
            else
            {
                on_5_2 = true;
                stop_vel_motors(target_vel_5,10);
            }
        }

        //id 2 id 12
        if(mov[6] == 1 && mov[7] == 1) //if(estadoTeclado[SDL_SCANCODE_U])
        {
            if(target_vel_2 < 180)
            {
                target_vel_2+=60;
                on_2_1  = true;  
            }
            else if(target_vel_2 == 180)
            {
                on_2_1  = false;  
            }
            if(target_vel_12 < 180)
            {
                target_vel_12+=60;
                on_12_1 = true;  
            }
            else if(target_vel_12 == 180)
            {
                on_12_1 = false;  
            }
        }
        else if(mov[6] == 2 && mov[7] == 2) //else if(!estadoTeclado[SDL_SCANCODE_O])
        {
            if(target_vel_2 > 0)
            {
                target_vel_2 -=60;
                on_2_1  = true;
            }
            else if(target_vel_2 == 0)
            {
                on_2_1  = false;  
            }
            if(target_vel_12 > 0)
            {
                target_vel_12 -=60;
                on_12_1 = true;
            }
            else if(target_vel_12 == 0)
            {
                on_12_1 = false;  
            }
        }
        if(mov[6] == 3 && mov[7] == 3) //if(estadoTeclado[SDL_SCANCODE_O])
        {

                if(target_vel_2 > -180)
                {
                    target_vel_2-=60;
                    on_2_2 = true;
                }
                else if(target_vel_2 == -180)
                {
                    on_2_2  = false;  
                }
            else
            {
                on_2_2 = true;
                stop_vel_motors(target_vel_2,60);
            }

                if(target_vel_12 > -180)
                {
                    target_vel_12-=60;
                    on_12_2 = true;
                }
                else if(target_vel_12 == -180)
                {
                    on_12_2 = false;  
                }
            else
            {
                on_12_2 = true;
                stop_vel_motors(target_vel_12,60);   
            }
        }
        else if(mov[6] == 4 && mov[7] == 4) //else if(!estadoTeclado[SDL_SCANCODE_U])
        {
            if(target_vel_2 < 0)
            {
                target_vel_2 +=60;
                on_2_2 = true;
            }
            else if(target_vel_2 == 0)
            {
                on_2_2  = false;  
            }
            if(target_vel_12 < 0)
            {
                target_vel_12 +=60;
                on_12_2 = true;
            }
            else if(target_vel_12 == 0)
            {
                on_12_2 = false;  
            }
        }   
    }
    else
    {
        Running = 0;
    }
}

void move_arm_motors(RoboArm *arm, Var_motors *var_m)
{
    /*Esta función se encarga de asignar la velocidad correspondiente para luego ser usada
      en la actuación de los motores de rozum*/

    // Motor 1
    if (mov[0] == 1) //if (estadoTeclado[SDL_SCANCODE_D])
    {
        if(var_m->p.motor_1 > -103*0.9856)
        {
            if(var_m->v.motor_1 > -vel_max)
            {
                var_m->v.motor_1-=acc_i;
            }
        }
        else
        {
            if(var_m->v.motor_1 < 0)
            {
                var_m->v.motor_1+=acc_f;
            }
        }
    }
    else if(mov[0] == 2) //else if(!estadoTeclado[SDL_SCANCODE_A])
    {
        stop_vel_motors(var_m->v.motor_1,acc_f);
    } 
    if (mov[0] == 3) //if (estadoTeclado[SDL_SCANCODE_A])
    {
        if(var_m->p.motor_1 < 72*0.9969)
        {
            if(var_m->v.motor_1 < vel_max)
            {
                var_m->v.motor_1 += acc_i; 
            }
        }
        else
        {
            if(var_m->v.motor_1 > 0)
            {
                var_m->v.motor_1-=acc_f;
            }
        }
    }
    else if(mov[0] == 4) //else if(!estadoTeclado[SDL_SCANCODE_D])
    {
        stop_vel_motors(var_m->v.motor_1,acc_f);
    }

    // Motor 2
    if (mov[1] == 1) //if (estadoTeclado[SDL_SCANCODE_E])
    {
        if(var_m ->p.motor_2 > -121*0.9874)
        {
            if(var_m->v.motor_2 > -vel_max)
            {
                var_m->v.motor_2-=acc_i;
            }
        }
        else
        {
            if(var_m->v.motor_2 < 0.0000)
            {
                var_m->v.motor_2+=acc_f;
            }
        }
    }
    else if(mov[1] == 2) //else if(!estadoTeclado[SDL_SCANCODE_Q])
    {
        stop_vel_motors(var_m->v.motor_2,acc_f);
    } 
    if (mov[1] == 3) //if (estadoTeclado[SDL_SCANCODE_Q])
    {
        if(var_m ->p.motor_2 < -27/0.9518)
        {
            if(var_m->v.motor_2 < vel_max)
            {
                var_m->v.motor_2 += acc_i; 
            }
        }
        else
        {
            if(var_m->v.motor_2 > 0.0000)
            {
                var_m->v.motor_2-=acc_f;
            }
        }
    }
    else if(mov[1] == 4) //else if(!estadoTeclado[SDL_SCANCODE_E])
    {
        stop_vel_motors(var_m->v.motor_2,acc_f);
    }
    
    
    // Motor 3
    if (mov[2] == 1) //if (estadoTeclado[SDL_SCANCODE_W])
    {
        if(var_m->p.motor_3 > -193*0.9933)
        {
            if(var_m->v.motor_3 > -vel_max)
            {
                var_m->v.motor_3-=acc_i;
            }
        }
        else
        {
            if(var_m->v.motor_3 < 0)
            {
                var_m->v.motor_3+=acc_f;
            }   
        }
    } 
    else 
    {
        if(mov[2] == 2) //else if(!estadoTeclado[SDL_SCANCODE_S])
        {
            stop_vel_motors(var_m->v.motor_3,acc_f);
        }
    }
    if (mov[2] == 3) //if (estadoTeclado[SDL_SCANCODE_S])
    {
        if(var_m->p.motor_3 > -40*0.965)
        {
            if(var_m->v.motor_3 < vel_max)
            {
                var_m->v.motor_3 += acc_i;
            }
        }
        else
        {
            if(var_m->v.motor_3 > 0)
            {
                var_m->v.motor_3-=acc_f;
            }
        }
    }
    else 
    {
        if(mov[2] == 4) //else if(!estadoTeclado[SDL_SCANCODE_W])
        {
            stop_vel_motors(var_m->v.motor_3,acc_f);
        }
    }
    if(estadoTeclado[SDL_SCANCODE_M])
    {
        if(!flag1)
        {
            vel_max+= 0.5;
            flag1 = true;
        }
    }
    else
    {
        flag1 = false;
    }
    if(estadoTeclado[SDL_SCANCODE_N])
    {
        if(!flag2)
        {
            if(vel_max > 0)
            {
                vel_max-= 0.5;
            }
            flag2 = true;
        }
    }
    else
    {
        flag2 = false;
    }

    if (estadoTeclado[SDL_SCANCODE_ESCAPE]) Running = 0;
}

void actuation_arm(RoboArm *arm, Var_motors *var_m)
{
    /*Esta función se encarga de la actuación de los motores de rozum haciendo que estos 
      se muevan a la velocidad asignada*/

    rr_set_velocity(arm->motor_1,var_m->v.motor_1);
    rr_set_velocity(arm->motor_2,var_m->v.motor_2);
    rr_set_velocity(arm->motor_3,var_m->v.motor_3);
}

void actuation_claw(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler,dynamixel::GroupBulkWrite groupBulkWrite)
{
    /*Esta función se encarga de la actuación de los motores dynamixel haciendo que estos 
      se muevan a la velocidad asignada*/ 
    /*
    //id 1
    if(on_1_1 || on_1_2)
    {
        packetHandler->write4ByteTxRx(portHandler, ID_1, GOAL_VELOCITY_ADRESS, uint32_t(target_vel_1));
    }
    // id 12 y id 2
    if((on_2_1 || on_2_2) || (on_12_1 || on_12_2))
    {
        packetHandler->write4ByteTxRx(portHandler, ID_2, GOAL_VELOCITY_ADRESS, uint32_t(target_vel_2));
        packetHandler->write4ByteTxRx(portHandler, ID_12, GOAL_VELOCITY_ADRESS, uint32_t(target_vel_12));
    }
    //id 3
    if(on_3_1 || on_3_2)
    {
        packetHandler->write4ByteTxRx(portHandler, ID_3, GOAL_VELOCITY_ADRESS, uint32_t(target_vel_3));
    }
    //id 5
    if(on_5_1 || on_5_2)
    {
        packetHandler->write4ByteTxRx(portHandler, ID_5, GOAL_VELOCITY_ADRESS, uint32_t(target_vel_5));
    }*/
    

    param_goal_velocity1[0] = DXL_LOBYTE(DXL_LOWORD(target_vel_1));
    param_goal_velocity1[1] = DXL_HIBYTE(DXL_LOWORD(target_vel_1));
    param_goal_velocity1[2] = DXL_LOBYTE(DXL_HIWORD(target_vel_1));
    param_goal_velocity1[3] = DXL_HIBYTE(DXL_HIWORD(target_vel_1));
    
    param_goal_velocity3[0] = DXL_LOBYTE(DXL_LOWORD(target_vel_3));
    param_goal_velocity3[1] = DXL_HIBYTE(DXL_LOWORD(target_vel_3));
    param_goal_velocity3[2] = DXL_LOBYTE(DXL_HIWORD(target_vel_3));
    param_goal_velocity3[3] = DXL_HIBYTE(DXL_HIWORD(target_vel_3));

    param_goal_velocity5[0] = DXL_LOBYTE(DXL_LOWORD(target_vel_5));
    param_goal_velocity5[1] = DXL_HIBYTE(DXL_LOWORD(target_vel_5));
    param_goal_velocity5[2] = DXL_LOBYTE(DXL_HIWORD(target_vel_5));
    param_goal_velocity5[3] = DXL_HIBYTE(DXL_HIWORD(target_vel_5));


    param_goal_velocity2[0] = DXL_LOBYTE(DXL_LOWORD(target_vel_2));
    param_goal_velocity2[1] = DXL_HIBYTE(DXL_LOWORD(target_vel_2));
    param_goal_velocity2[2] = DXL_LOBYTE(DXL_HIWORD(target_vel_2));
    param_goal_velocity2[3] = DXL_HIBYTE(DXL_HIWORD(target_vel_2));
    
    param_goal_velocity12[0] = DXL_LOBYTE(DXL_LOWORD(target_vel_12));
    param_goal_velocity12[1] = DXL_HIBYTE(DXL_LOWORD(target_vel_12));
    param_goal_velocity12[2] = DXL_LOBYTE(DXL_HIWORD(target_vel_12));
    param_goal_velocity12[3] = DXL_HIBYTE(DXL_HIWORD(target_vel_12));
    
    groupBulkWrite.addParam(ID_1,GOAL_VELOCITY_ADDRESS,4, param_goal_velocity1);
    groupBulkWrite.addParam(ID_3, GOAL_VELOCITY_ADDRESS,4,param_goal_velocity3);
    groupBulkWrite.addParam(ID_5, GOAL_VELOCITY_ADDRESS,4,param_goal_velocity5);
    groupBulkWrite.addParam(ID_2, GOAL_VELOCITY_ADDRESS,4,param_goal_velocity2);
    groupBulkWrite.addParam(ID_12, GOAL_VELOCITY_ADDRESS,4,param_goal_velocity12);

    groupBulkWrite.txPacket();
    groupBulkWrite.clearParam();
}

void read_variables_arm(RoboArm *arm, Var_motors *var_m)
{
    //Esta función se encarga de leer todas las variables disponibles de los motores de rozum

    //Update cache of Arm Motors
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);
    //Read parameters of Arm Motors
    rr_read_cached_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_cached_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_cached_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);
    rr_read_cached_parameter(arm->motor_1,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_1);
    rr_read_cached_parameter(arm->motor_2,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_2);
    rr_read_cached_parameter(arm->motor_3,APP_PARAM_CURRENT_INPUT,&var_m->c.motor_3);
    rr_read_cached_parameter(arm->motor_1,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_1);
    rr_read_cached_parameter(arm->motor_2,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_2);
    rr_read_cached_parameter(arm->motor_3,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m->t.motor_3);  
}

void read_variables_claw(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler,dynamixel::GroupBulkRead groupBulkRead)
{
    //Esta función se encarga de leer de los registros de cada motor dynamixel su posición y velocidad
    /*
    //Read Claw Position Motors
    packetHandler->read4ByteTxRx(portHandler, ID_1, PRESENT_POSITION_ADRESS, &read_pos_1);
    packetHandler->read4ByteTxRx(portHandler, ID_12, PRESENT_POSITION_ADRESS, &rpos_12);
    packetHandler->read4ByteTxRx(portHandler, ID_2, PRESENT_POSITION_ADRESS, &rpos_2);
    packetHandler->read4ByteTxRx(portHandler, ID_3, PRESENT_POSITION_ADRESS, &read_pos_3);
    packetHandler->read4ByteTxRx(portHandler, ID_5, PRESENT_POSITION_ADRESS, &read_pos_5);

    //Read Claw Velocity Motors
    packetHandler->read4ByteTxRx(portHandler, ID_1, PRESENT_VELOCITY_ADRESS, &read_vel_1);
    packetHandler->read4ByteTxRx(portHandler, ID_12, PRESENT_VELOCITY_ADRESS, &read_vel_12);
    packetHandler->read4ByteTxRx(portHandler, ID_2, PRESENT_VELOCITY_ADRESS, &read_vel_2);
    packetHandler->read4ByteTxRx(portHandler, ID_3, PRESENT_VELOCITY_ADRESS, &read_vel_3);
    packetHandler->read4ByteTxRx(portHandler, ID_5, PRESENT_VELOCITY_ADRESS, &read_vel_5);
    */
   groupBulkRead.addParam(ID_1,PRESENT_VELOCITY_ADDRESS,8);
   groupBulkRead.addParam(ID_3,PRESENT_VELOCITY_ADDRESS,8);
   groupBulkRead.addParam(ID_5,PRESENT_VELOCITY_ADDRESS,8);
   groupBulkRead.addParam(ID_2,PRESENT_VELOCITY_ADDRESS,8);
   groupBulkRead.addParam(ID_12,PRESENT_VELOCITY_ADDRESS,8);
    groupBulkRead.txRxPacket();
    
    read_vel_1 = groupBulkRead.getData(ID_1,PRESENT_VELOCITY_ADDRESS,4);
    read_vel_3 = groupBulkRead.getData(ID_3,PRESENT_VELOCITY_ADDRESS,4);
    read_vel_5 = groupBulkRead.getData(ID_5,PRESENT_VELOCITY_ADDRESS,4);
    read_vel_2 = groupBulkRead.getData(ID_2,PRESENT_VELOCITY_ADDRESS,4);
    read_vel_12 = groupBulkRead.getData(ID_12,PRESENT_VELOCITY_ADDRESS,4);
    read_pos_1 = groupBulkRead.getData(ID_1,PRESENT_POSITION_ADDRESS,4);
    read_pos_3 = groupBulkRead.getData(ID_3,PRESENT_POSITION_ADDRESS,4);
    read_pos_5 = groupBulkRead.getData(ID_5,PRESENT_POSITION_ADDRESS,4);
    rpos_2 = groupBulkRead.getData(ID_2,PRESENT_POSITION_ADDRESS,4);
    rpos_12 = groupBulkRead.getData(ID_12,PRESENT_POSITION_ADDRESS,4);

}

void power_off(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
    /*Esta función se encarga de deshabilitar o apagar los motores dynamixel cuando el programa acabe
      (Solo existe esta función para los dynamixel dado que los motores de Rozum dejan de funcionar automaticamente cuando el programa acaba)*/
    
    //id 1
    packetHandler->write1ByteTxRx(portHandler, ID_1, TORQUE_ADDRESS, OFF);
    //id 2
    packetHandler->write1ByteTxRx(portHandler, ID_2, TORQUE_ADDRESS, OFF);

    //id 3
    packetHandler->write1ByteTxRx(portHandler, ID_3, TORQUE_ADDRESS, OFF);

    //id 5
    packetHandler->write1ByteTxRx(portHandler, ID_5, TORQUE_ADDRESS, OFF);

    //id 12
    packetHandler->write1ByteTxRx(portHandler, ID_12, TORQUE_ADDRESS, OFF);
}

void stop_motors()
{
    /*Esta función se encarga de parar todos los motores independientemente
      del estado en el que se encuentran*/

    for (int i = 0; i < 8; i++)
    {
        //Motor n
        if(mov[i] == 1)
        {
            mov[i] = 2;
        }
        else if (mov[i] == 3)
        {
            mov[i] = 4;
        }
    }
}

void read_controller()
{

    //Esta función se encarga de leer las entradas del mando para ejecutar una accion dependiendo del boton que se presione o suelte

    //Motor 1
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 20000)
    {
        if(!pausa)mov[0] = 1;
    }
    else
    {
        if(mov[0] == 1)
        {
            mov[0] = 2;
        }
    }
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 20000)
    {
        if(!pausa)mov[0] = 3;
    }
    else
    {
        if(mov[0] == 3)
        {
            mov[0] = 4;
        }
    }

    //Motor 2
    if (SDL_GameControllerGetButton(mando, SDL_CONTROLLER_BUTTON_B)) {
        if(!pausa)mov[1] = 1;
    }
    else
    {
        if(mov[1] == 1)
        {
            mov[1] = 2;
        }
    }
    if (SDL_GameControllerGetButton(mando, SDL_CONTROLLER_BUTTON_X)) {
        if(!pausa)mov[1] = 3;
    }
    else
    {
        if(mov[1] == 3)
        {
            mov[1] = 4;
        }
    }
    
    //Motor 3
    if (SDL_GameControllerGetButton(mando, SDL_CONTROLLER_BUTTON_Y)) {
        if(!pausa)mov[2] = 1;
    }
    else
    {
        if(mov[2] == 1)
        {
            mov[2] = 2;
        }
    }
    if (SDL_GameControllerGetButton(mando, SDL_CONTROLLER_BUTTON_A)) {
        if(!pausa)mov[2] = 3;
    }
    else
    {
        if(mov[2] == 3)
        {
            mov[2] = 4;
        }
    }

    //Motor 4
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_LEFTX) < -20000)
    {
        if(!pausa)mov[3] = 1;
    }
    else
    {
        if(mov[3] == 1)
        {
            mov[3] = 2;
        }
    }
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_LEFTX) > 20000)
    {
        if(!pausa)mov[3] = 3;
    }
    else
    {
        if(mov[3] == 3)
        {
            mov[3] = 4;
        }
    }

    //Motor 5
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_LEFTY) < -20000)
    {
        if(!pausa)mov[4] = 1;
    }
    else
    {
        if(mov[4] == 1)
        {
            mov[4] = 2;
        }
    }
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_LEFTY) > 20000)
    {
        if(!pausa)mov[4] = 3;
    }
    else
    {
        if(mov[4] == 3)
        {
            mov[4] = 4;
        }
    }

    //Motor 4
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_RIGHTX) < -20000)
    {
        if(!pausa)mov[5] = 1;
    }
    else
    {
        if(mov[5] == 1)
        {
            mov[5] = 2;
        }
    }
    if(SDL_GameControllerGetAxis(mando, SDL_CONTROLLER_AXIS_RIGHTX) > 20000)
    {
        if(!pausa)mov[5] = 3;
    }
    else
    {
        if(mov[5] == 3)
        {
            mov[5] = 4;
        }
    }

    //Motor 6 y 7
    if (SDL_GameControllerGetButton(mando, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) {
        if(!pausa)
        {
            mov[6] = 1;
            mov[7] = 1;
        }
    }
    else
    {
        if(mov[6] == 1 && mov[7] == 1)
        {
            mov[6] = 2;
            mov[7] = 2;
        }
    }
    if (SDL_GameControllerGetButton(mando, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        if(!pausa)
        {
            mov[6] = 3;
            mov[7] = 3;
        }
    }
    else
    {
        if(mov[6] == 3 && mov[7] == 3)
        {
            mov[6] = 4;
            mov[7] = 4;
        }
    }
}

void Dynamixel_task(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler,dynamixel::GroupBulkWrite groupBulkWrite,dynamixel::GroupBulkRead groupBulkRead)
{
    //Esta función ejecuta de forma paralela las acciones de mover los motores dynamixel

    /*Esto es debido a que la transmision y recepcion de datos afectan a la latencia del programa principal
      lo que generaba retrasos en las acciones de mover o parar los motores del manipulador*/ 
    
    while(Running)
    {
        
        //Modifica las variables de velocidad de los motores
        move_claw_motors(portHandler,packetHandler);
        
        //Pone en marcha los motores con su velocidad correspondiente anteriormente asignada
        actuation_claw(portHandler,packetHandler, groupBulkWrite);
        
        //Lee las variables de los motores
        read_variables_claw(portHandler,packetHandler,groupBulkRead);
    }
}

void Manipulator::full_manual_mode()
{
    //Inicializamos el estado de los motores
    init_mov();

    //Inicializamos todos los motores del manipulador
    init_motors(&arm,&var_m,portHandler,packetHandler);
    //Iniciamos el proceso de la ventana
    //dynamixel::GroupSyncWrite groupSyncWrite(portHandler, packetHandler, GOAL_VELOCITY_ADRESS, 4);
    //dynamixel::GroupSyncRead groupSyncRead(portHandler, packetHandler, PRESENT_POSITION_ADRESS, 4);
    //dynamixel::GroupSyncRead groupSyncRead2(portHandler, packetHandler, PRESENT_VELOCITY_ADRESS, 4);

    dynamixel::GroupBulkWrite groupBulkWrite(portHandler, packetHandler);
    dynamixel::GroupBulkRead groupBulkRead(portHandler, packetHandler);

    init_win();

    //Iniciamos el hilo de los motores dynamixel
    std::thread garra(Dynamixel_task,portHandler,packetHandler,groupBulkWrite,groupBulkRead);
    getIDS(idss);
    for (int i = 0; i < 8; i++)
    {
        printf("%hhn\n",idss[i]);
    }
    
    //Iniciamos el bucle principal
    while (Running) {
        while (SDL_PollEvent(&evento)) {
            // Detectar si se conecta el mando
            if (evento.type == SDL_QUIT) Running = 0;
            else if (evento.type == SDL_CONTROLLERDEVICEADDED) {
                if (!mando) {
                    mando = SDL_GameControllerOpen(evento.cdevice.which);
                    std::cout << "Mando conectado [Running]." << std::endl;
                }
            }
            // Detectar si se desconecta el mando
            else if (evento.type == SDL_CONTROLLERDEVICEREMOVED) {
                if (mando && evento.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(mando))) {
                    SDL_GameControllerClose(mando);
                    mando = nullptr;
                    std::cout << "Mando desconectado." << std::endl;
                }
            }
            // Leer botones por evento
            else if (evento.type == SDL_CONTROLLERBUTTONDOWN) {
                
                if (evento.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    pausa = !pausa;
                    if(pausa) stop_motors();
                }
                if(evento.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                {
                    acc_i += 0.5;
                }
                else if(evento.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                {
                    acc_i -= 0.5;
                }
                if(evento.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                {
                    acc_f += 0.5;
                }
                else if(evento.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                {
                    acc_f -= 0.5;
                }
            }
        }

        //Leer variables de motores de rozum
        read_variables_arm(&arm,&var_m);

        //Leer entradas del mando o teclado
        if(!mando)
        {
            read_keyboard();
        }
        else
        {
            read_controller();
        }

        //Modificar la velocidad de motores de rozum segun las entradas
        move_arm_motors(&arm,&var_m);

        //Poner en marcha los motores de rozum segun la velocidad asignada anteriormente
        actuation_arm(&arm,&var_m);
        
        //Imprimir la información en la ventana
        print_variables(&arm,&var_m);

        //Retardo de 20ms para darle tiempo al sistema y a los motores de responder a la actuación
        SDL_Delay(20);
    }

    //Cierre de la tarea de los motores dynamixel
    garra.join();

    //Terminamos el proceso de la ventana
    finish_win();

    //Desactivamos los motores dynamixel
    power_off(portHandler,packetHandler);
    //finish_motors_arm(&arm);
}