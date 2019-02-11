#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/ipc.h>
#include<semaphore.h>

// Número de asientos disponibles en la barbería
#define total_clientes 15

time_t end_time;
// Semáforos
sem_t mutex,cliente_listo,barbero_listo;
// Número de clientes esperando
int count = 0;
// Número de asientos disponibles
int asientos_disponibles = 0;

void barbero(void *arg);
void cliente(void *arg);

int main(int argc,char *argv[])
{
	pthread_t id1,id2;

	// El programa debe recibir un argumento
  if (argc != 2)
  {
    printf("Modo de uso: BarberoDurmiente <Num Sillas>\n");
    exit(-1);
  }

  // Recibir el número n de clientes que pueden esperar simultáneamente (sillas)
  asientos_disponibles = atoi(argv[1]);

	// Inicializar semáforos
	sem_init(&mutex,0,1);
	sem_init(&cliente_listo,0,0);
	sem_init(&barbero_listo,0,1);

	// Inicializar el hilo del barbero
	pthread_create(&id1,NULL,(void *)barbero,NULL);

	// Inicializar el hilo del cliente
	pthread_create(&id2,NULL,(void *)cliente,NULL);

	// Esperar a que terminen su ejecución
	pthread_join(id2,NULL);
	pthread_join(id1,NULL);
}

/**
* Función que define el comportamiento del hilo barbero
*/
void barbero(void *arg)
{
	int i = 0;
	// Mientras no haya clientes
	while(i <= total_clientes)
	{
		// Espera a que llegue un cliente, mientras se duerme
		sem_wait(&cliente_listo);
		// Trata de modificar el número de clientes esperando
		sem_wait(&mutex);
		count--;
		// Listo para cortar
		sem_post(&mutex);
		sem_post(&barbero_listo);
		sleep(5);
		i++;
	}
	printf("Todos los clientes se han retirado. El barbero cierra.\n");
}

/**
* Función que define el comportamiento del hilo cliente
*/
void cliente(void *arg)
{
	int id_cliente = 1;
	while(id_cliente <= total_clientes)
	{
		printf("El cliente %d quiere recibir un corte de cabello\n", id_cliente);
		// Tratar de obtener acceso a un lugar
		sem_wait(&mutex);
		// Si hay asientos disponibles
		if(count < asientos_disponibles)
		{
			// Aumentar el número de clientes en espera
			count++;
			// Liberar acceso a los lugares
			sem_post(&mutex);
			// Notificar al barbero
			sem_post(&cliente_listo);
			// Esperar a que el barbero esté listo
			printf("El cliente %d espera al barbero. Clientes en la lista: %d\n", id_cliente, count);
			sem_wait(&barbero_listo);
			printf("Barbero cortando el cabello a un cliente...\n",count);
		}
		else
		{
			printf("El cliente %d se marcha sin recibir corte\n", id_cliente);
			// No hay asientos disponibles
			sem_post(&mutex);
		}
		printf("El cliente %d termina de ser atendido y se marcha\n", id_cliente);
		sleep(2);
		id_cliente++;
	}
}
