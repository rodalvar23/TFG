#include "../include/robotic_arm.h"

//motor 1 intervalo de posiciones  72   a  -103 
//motor 2 intervalo de posiciones -127  a  -27
//motor 3 intervalo de posiciones -194  a  -28


//Variables Gráficas

SDL_Window* ventana;
SDL_Renderer* renderizador;
TTF_Font* fuente;
SDL_Event evento;
SDL_Color colorTexto = {255, 255, 255, 255};

int Running = 1;
int contador = 0; // Nuestra variable a mostrar 
int espacioPresionado = 0; // Variable para evitar que el contador sume miles de veces por segundo si mantenemos presionado
const Uint8 *estadoTeclado;


int init_win()
{
    // 1. Inicializar SDL y TTF
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    // Iniciar el subsistema de fuentes
    if (TTF_Init() == -1) {
        printf("Error TTF: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    ventana = SDL_CreateWindow("Control del Manipulador Robótico", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, 0);
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
    TTF_CloseFont(fuente);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(ventana);
    TTF_Quit(); // Apagar el subsistema de fuentes
    SDL_Quit();
}

void print_table()
{
    // 1. Configuración de la tabla
        int filas = 2;
        int columnas = 2;
        int anchoCelda = 580;
        int altoCelda = 70;
        int xInicial = 10;  // Margen izquierdo
        int yInicial = 80; // Margen superior
        int yActual;
        int xActual;
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
                yActual = yInicial + (i * (altoCelda+25));
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
                               xActual, yInicial + (filas * (altoCelda+125))); // Punto final (X, Y)
        }


        for (int i = 0; i <= filas; i++) {
            if(i < filas)
            {
                yActual = yInicial+190 + (i * altoCelda);
            }
            else if(i == filas)
            {
                yActual = yInicial+200 + (i * (altoCelda+25));
            }
            SDL_RenderDrawLine(renderizador, 
                               xInicial, yActual, // Punto de inicio (X, Y)
                               xInicial + (columnas * anchoCelda), yActual); // Punto final (X, Y)
        }   
}

void print_variables(RoboArm arm, Var_motors var_m)
{
        print_table();
        // B. Preparar el texto con nuestra variable
        char textoAMostrar[1300];
        rr_param_cache_update(arm.motor_1);
        rr_param_cache_update(arm.motor_2);
        rr_param_cache_update(arm.motor_3);

        rr_read_cached_parameter(arm.motor_1,APP_PARAM_CURRENT_INPUT,&var_m.c.motor_1);
        rr_read_cached_parameter(arm.motor_2,APP_PARAM_CURRENT_INPUT,&var_m.c.motor_2);
        rr_read_cached_parameter(arm.motor_3,APP_PARAM_CURRENT_INPUT,&var_m.c.motor_3);

        rr_read_cached_parameter(arm.motor_1,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m.t.motor_1);
        rr_read_cached_parameter(arm.motor_2,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m.t.motor_2);
        rr_read_cached_parameter(arm.motor_3,APP_PARAM_TEMPERATURE_ACTUATOR,&var_m.t.motor_3);

        // snprintf es como printf, pero guarda el texto en la variable textoAMostrar
        snprintf(textoAMostrar, sizeof(textoAMostrar),
            "\n"
            "                                     ROBOTIC MANIPULATOR INFORMATION\n\n" 
            "   Position Variables:                                      Velocity Variables:\n\n" 
            "   Position (DEG) motor 1:  %f          Velocity (DEG/s) motor 1: %f\n"
            "   Position (DEG) motor 2:  %f        Velocity (DEG/s) motor 2: %f\n"
            "   Position (DEG) motor 3:  %f        Velocity (DEG/s) motor 3: %f\n\n"
            "   Temperature Variables:                              Current Variables:\n\n" 
            "   Temperature (C) motor 1:  %f        Current (A) motor 1: %f\n"
            "   Temperature (C) motor 2:  %f        Current (A) motor 2: %f\n"
            "   Temperature (C) motor 3:  %f        Current (A) motor 3: %f\n"
            ,var_m.p.motor_1,var_m.v.motor_1,var_m.p.motor_2,var_m.v.motor_2,var_m.p.motor_3,var_m.v.motor_3,
            var_m.t.motor_1,var_m.c.motor_1,var_m.t.motor_2,var_m.c.motor_2,var_m.t.motor_3,var_m.c.motor_3);

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

        SDL_Delay(16);
}

void move_motors(RoboArm *arm, Var_motors *var_m)
{
    estadoTeclado = SDL_GetKeyboardState(NULL);
    rr_param_cache_update(arm->motor_1);
    rr_param_cache_update(arm->motor_2);
    rr_param_cache_update(arm->motor_3);
    rr_read_cached_parameter(arm->motor_1,APP_PARAM_POSITION,&var_m->p.motor_1);
    rr_read_cached_parameter(arm->motor_2,APP_PARAM_POSITION,&var_m->p.motor_2);
    rr_read_cached_parameter(arm->motor_3,APP_PARAM_POSITION,&var_m->p.motor_3);
    // Motor 1
    if (estadoTeclado[SDL_SCANCODE_D])
    {
        if(var_m->p.motor_1 > -103)
        {
            if(var_m->v.motor_1 > -5)
            {
                var_m->v.motor_1-=0.5;
            }
        }
        else
        {
            if(var_m->v.motor_1 < 0)
            {
                var_m->v.motor_1+=0.5;
            }
        }
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_A])
        {
            if(var_m->v.motor_1 < 0)
            {
                var_m->v.motor_1+=0.5;
            }
        }
    } 
    if (estadoTeclado[SDL_SCANCODE_A])
    {
        if(var_m->p.motor_1 < 72)
        {
            if(var_m->v.motor_1 < 5)
            {
                var_m->v.motor_1 += 0.5; 
            }
        }
        else
        {
            if(var_m->v.motor_1 > 0)
            {
                var_m->v.motor_1-=0.5;
            }
        }
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_D])
        {
            if(var_m->v.motor_1 > 0)
            {
                var_m->v.motor_1-=0.5;
            }
        }
    }

    // Motor 2
    if (estadoTeclado[SDL_SCANCODE_E])
    {
        if(var_m ->p.motor_2 > -121)
        {
            if(var_m->v.motor_2 > -5)
            {
                var_m->v.motor_2-=0.5;
            }
        }
        else
        {
            if(var_m->v.motor_2 < 0.0000)
            {
                var_m->v.motor_2+=0.5;
            }
        }
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_Q])
        {
            if(var_m->v.motor_2 < 0.0000)
            {
                var_m->v.motor_2+=0.5;
            }
        }
    } 
    if (estadoTeclado[SDL_SCANCODE_Q])
    {
        if(var_m ->p.motor_2 < -27)
        {
            if(var_m->v.motor_2 < 5)
            {
                var_m->v.motor_2 += 0.5; 
            }
        }
        else
        {
            if(var_m->v.motor_2 > 0.0000)
            {
                var_m->v.motor_2-=0.5;
            }
        }
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_E])
        {
            if(var_m->v.motor_2 > 0.0000)
            {
                var_m->v.motor_2-=0.5;
            }
        }
    }
    
    
    // Motor 3
    if (estadoTeclado[SDL_SCANCODE_W])
    {
        if(var_m->p.motor_3 > -193)
        {
            if(var_m->v.motor_3 > -5)
            {
                var_m->v.motor_3-=0.5;
            }
        }
        else
        {
            if(var_m->v.motor_3 < 0)
            {
                var_m->v.motor_3+=0.5;
            }   
        }
    } 
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_S])
        {
            if(var_m->v.motor_3 < 0)
            {
                var_m->v.motor_3+=0.5;
            }
        }
    }
    if (estadoTeclado[SDL_SCANCODE_S])
    {
        if(var_m->p.motor_3 < -40)
        {
            if(var_m->v.motor_3 < 5)
            {
                var_m->v.motor_3 += 0.5;
            }
        }
        else
        {
            if(var_m->v.motor_3 > 0)
            {
                var_m->v.motor_3-=0.5;
            }
        }
    }
    else
    {
        if(!estadoTeclado[SDL_SCANCODE_W])
        {
            if(var_m->v.motor_3 > 0)
            {
                var_m->v.motor_3-=0.5;
            }
        }
    }


    if (estadoTeclado[SDL_SCANCODE_ESCAPE]) Running = 0;
    rr_set_velocity(arm->motor_1,var_m->v.motor_1);
    rr_set_velocity(arm->motor_2,var_m->v.motor_2);
    rr_set_velocity(arm->motor_3,var_m->v.motor_3); 
}

void manual_mode(RoboArm arm, Var_motors var_m)
{
    init_win();
    while (Running) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) Running = 0;
        }
        move_motors(&arm,&var_m);
        print_variables(arm,var_m);
    }
    finish_win();
}