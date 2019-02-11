/* Implementacion de semaforo
* contador es el valor del semaforo
* wakeups cuenta el numero de hilos que estan despiertos, pero aun no continuan con su ejecucion
* mutex provee acceso exclusivo a las variables anteriores
* cond es la variable condicional que los hilos esperan
*/
typedef struct {
  int contador, wakeups;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;
} semaforo;

semaforo *crea_semaforo(int valor);
void wait(semaforo *sem);
void signal(semaforo *sem);
