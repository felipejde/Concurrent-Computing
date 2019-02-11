#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5     // Número de filósofos
#define ITERACIONES 5       // Número de iteraciones por filósofo

// Variable global para los semáforos de cada palillo
omp_lock_t palillos[NUM_FILOSOFOS];

// Variable global que representa al mesero
omp_lock_t mesero;

int main(int argc, char** argv)
{
  int idHilo;
  int i;

  // Inicializar los candados de los palillos
  for(i = 0; i < NUM_FILOSOFOS; i++)
    omp_init_lock(&palillos[i]);

  // Inicializar el candado del mesero
  omp_init_lock(&mesero);

  // Establecer el número de hilos (filósofos)
  omp_set_num_threads(NUM_FILOSOFOS);

  // Sección paralela
  #pragma omp parallel private(idHilo)
  {
    idHilo = omp_get_thread_num();

    // Identificadores de los palillos para el filósofo
    int palillo_izq, palillo_der;

    // Establecer el identificador de cada palillo
    if(idHilo < NUM_FILOSOFOS -1)
    {
      palillo_izq = idHilo + 1;
      palillo_der = idHilo;
    }
    // Para el último filósofo, su palillo izquierdo es el palillo derecho del primero
    else
    {
      palillo_izq = 0;
      palillo_der = idHilo;
    }

    int j;
    // El filósofo hace lo siguiente 5 veces:
    for(j = 0; j < ITERACIONES; j++)
    {
      printf("\nEl filosofo %d esta pensando...\n", idHilo + 1);
      usleep(1000000);

      printf("\nEl filosofo %d tiene hambre...\n", idHilo + 1);

      // Pregunta al mesero por disponibilidad
      omp_set_lock(&mesero);

      // Intenta acceder al palillo izquierdo
      omp_set_lock(&palillos[palillo_izq]);
      // Intenta acceder al palillo derecho
      omp_set_lock(&palillos[palillo_der]);

      printf("\nEl filosofo %d esta comiendo...\n", idHilo + 1);
      usleep(1000000);

      printf("\nEl filosofo %d termino de comer...\n", idHilo + 1);

      // Libera el palillo izquierdo
      omp_unset_lock(&palillos[palillo_izq]);
      // Libera el palillo derecho
      omp_unset_lock(&palillos[palillo_der]);

      // Le entrega los palillos al mesero
      omp_unset_lock(&mesero);
    }
    printf("\nEl filosofo %d se retira...\n", idHilo + 1);
  }

  printf("\nSe han retirado todos los filosofos...\n");

  // Destruir los candados
  for(i = 0; i < NUM_FILOSOFOS; i++)
    omp_destroy_lock(&palillos[i]);

  omp_destroy_lock(&mesero);

  return 0;
}
