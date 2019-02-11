#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include "semaforo.h"


/**
* Crea e inicializa un semaforo con un valor dado.
* @param valor el valor inicial
* @return el semaforo
*/
semaforo *crea_semaforo(int valor)
{
  semaforo *sem = malloc(sizeof(semaforo));
  sem->contador = valor;
  sem->wakeups = 0;

  pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(mutex, NULL);
  sem->mutex = mutex;

  pthread_cond_t *cond = malloc(sizeof(pthread_cond_t));
  pthread_cond_init(cond, NULL);
  sem->cond = cond;

  return sem;
}

/**
* Adquiere o bloquea un proceso y disminuye el valor del semaforo en uno.
* Operacion wait (P) del semaforo.
* @param sem el semaforo
*/
void wait(semaforo *sem)
{
  // Bloquea el acceso para los procesos
  pthread_mutex_lock(sem->mutex);
  sem->contador--;

  /* Si el contador es menor a cero, checa por la condición y reduce el numero
  de hilos que aun no continuan con su ejecucion */
  if (sem->contador < 0) {
    do {
      pthread_cond_wait(sem->cond, sem->mutex);
    } while (sem->wakeups < 1);
    sem->wakeups--;
  }
  // Desbloquea el acceso para los procesos
  pthread_mutex_unlock(sem->mutex);
}

/**
* Libera un proceso bloqueado y aumenta el valor del semaforo en uno.
* Operacion signal (V) del semaforo.
* @param sem el semaforo
*/
void signal(semaforo *sem)
{

  // Bloquea el acceso para los procesos
  pthread_mutex_lock(sem->mutex);
  sem->contador++;

  /* Si el contador es menor o igual a cero, aumenta el numero de hilos que
   estan despiertos pero no continuan su ejecucion. Además de desbloquear el
   hilo.
  */
  if (sem->contador <= 0) {
    sem->wakeups++;
    pthread_cond_signal(sem->cond);
  }
  // Desbloquea el acceso para los procesos
  pthread_mutex_unlock(sem->mutex);
}
