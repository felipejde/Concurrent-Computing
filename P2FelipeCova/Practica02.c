#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>
#include <semaphore.h>
#include "semaforo.h"

// Valor inicial del contador del semaforo
#define SEM_INICIAL		2

int main(int argc, char** argv)
{
  int numHilos, idHilo;

  //verifica el numero de hilos que se paso como parametro
  if (argc < 2)
  {
    printf("Es necesario pasar el numero de hilos como argumento!\n");
    exit(1);
  }

  sscanf(argv[1], "%i", &numHilos);

  if (numHilos < 1)
  {
    printf("Numero de hilos no valido: (%i)\n", numHilos);
    exit(1);
  }

  omp_set_num_threads(numHilos);
  // Semaforo compartido para los hilos
  semaforo *sem = crea_semaforo(SEM_INICIAL);

  //inicia seccion paralela
  #pragma omp parallel private(idHilo)
  {
    idHilo = omp_get_thread_num();

    int i;
    for(i = 0; i < 10; i++)
    {
      // bloquea al hilo en ejecucion
  		wait(sem);

  		/* Inicio de la sección especial */
  		printf("Hilo %d entrando a la seccion especial... \nS = %d\n\n",
        idHilo, sem->contador);

      int sum = 0;
      /* Suma de los primeros 1000 naturales */
      for(int j = 0; j < 1000; j++) { sum += j; }

  	  printf("Hilo %d saliendo de la seccion especial ...\n\n", idHilo);
  	  /* Fin de la sección especial */

  	  // desbloquea la hilo
  	  signal(sem);
  	}

  }//fin de seccion paralela
}
