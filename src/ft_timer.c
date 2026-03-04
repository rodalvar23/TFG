#include "../include/robotic_arm.h"


// Inicializa el cronómetro con el tiempo actual
void iniciar_timer_ms(TimerMS *t, long milisegundos) {
    clock_gettime(CLOCK_MONOTONIC, &(t->inicio));
    t->ms_objetivo = milisegundos;
}

// Comprueba si han pasado los milisegundos indicados
bool timer_ms_expirado(TimerMS *t) {
    struct timespec ahora;
    clock_gettime(CLOCK_MONOTONIC, &ahora);

    // Calculamos la diferencia
    long segundos = ahora.tv_sec - t->inicio.tv_sec;
    long nanosegundos = ahora.tv_nsec - t->inicio.tv_nsec;

    // Convertimos la diferencia total a milisegundos
    long ms_transcurridos = (segundos * 1000) + (nanosegundos / 1000000);

    return ms_transcurridos >= t->ms_objetivo;
}
