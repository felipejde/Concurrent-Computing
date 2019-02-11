#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

void dekker(int);
void peterson(int);
void kessels(int);

int main()
{
	int numHilos = 2;
	dekker(numHilos);
	printf("***********************************************\n");
	peterson(numHilos);
	printf("***********************************************\n");
	kessels(numHilos);
	printf("***********************************************\n");
}

/**
* Algoritmo de Dekker para exclusión mutua.
* @param numHilos el número de hilos compitiendo.
*/
void dekker(int numHilos)
{

	int idHilo;
	// Variables compartidas
	int turno;
	int flag[] = {0,0};

	omp_set_num_threads(numHilos);

	/* Algoritmo de Dekker */
	printf("\n************* Algoritmo de Dekker *************\n");
	//inicia seccion paralela
	#pragma omp parallel private(idHilo) shared(turno, flag)
	{
		idHilo = omp_get_thread_num();

		int i;
		// Realizar 20 iteraciones por hilo
		for(i = 0; i < 20; i++)
		{

			// Sección no crítica
	    /* identificador del otro hilo */
	    int otro = (idHilo + 1) % 2;
	    /* cambiar la bandera de este hilo a true */
	    flag[idHilo] = 1;
	    /* mientras la bandera del otro hilo sea true ... */
	    while(flag[otro] == 1) {
	      if(turno == otro) {
	        flag[idHilo] = 0;
	        while(turno != idHilo);
	        flag[idHilo] = 1;
	      }
	    }

			//indica que la siguiente seccion se realiza con acceso exclusivo
			#pragma omp critical
			{
				printf("El hilo %d entro a su seccion critica\n", idHilo);
		    int sum = 0;
		    /* Suma de los primeros 1000 naturales */
		    for(int j = 0; j < 1000; j++) { sum += j; }
		    printf("El hilo %d ha terminado su seccion critica\n", idHilo);
		    turno = otro;
		    flag[idHilo] = 0;
			}
		}
	}//fin de seccion paralela
}

/**
* Algoritmo de Peterson para exclusión mutua.
* @param numHilos el número de hilos compitiendo.
*/
void peterson(int numHilos)
{
	int idHilo;
	// Variables compartidas
	int turno;
	int flag[] = {0,0};

	omp_set_num_threads(numHilos);

	/* Algoritmo de Dekker */
	printf("\n************ Algoritmo de Peterson ************\n");
	//inicia seccion paralela
	#pragma omp parallel private(idHilo) shared(turno, flag)
	{
		idHilo = omp_get_thread_num();

		int i;
		// Realizar 20 iteraciones por hilo
		for(i = 0; i < 20; i++)
		{
			// Sección no crítica

			/* identificador del otro hilo */
	    int otro = (idHilo + 1) % 2;
	    /* cambiar la bandera de este hilo a true */
	    flag[idHilo] = 1;
	    turno = otro;
	    while(flag[otro] && turno == otro);

			//indica que la siguiente seccion se realiza con acceso exclusivo
			#pragma omp critical
			{
				printf("El hilo %d entro a su seccion critica\n", idHilo);
		    int sum = 0;
		    /* Suma de los primeros 1000 naturales */
		    for(int j = 0; j < 1000; j++) { sum += j; }
		    printf("El hilo %d ha terminado su seccion critica\n", idHilo);
		    flag[idHilo] = 0;
			}
		}
	}//fin de seccion paralela
}

/**
* Algoritmo de Kessels para exclusión mutua.
* @param numHilos el número de hilos compitiendo.
*/
void kessels(int numHilos)
{
	int idHilo;
	// Variables compartidas
	int turno[] = {0,0};
	int flag[] = {0,0};

	omp_set_num_threads(numHilos);

	/* Algoritmo de Dekker */
	printf("\n************* Algoritmo de Kessels ************\n");
	//inicia seccion paralela
	#pragma omp parallel private(idHilo) shared(turno, flag)
	{
		idHilo = omp_get_thread_num();

		int i;
		// Realizar 20 iteraciones por hilo
		for(i = 0; i < 20; i++)
		{

			// Sección no crítica
	    flag[idHilo] = 1;
	    /* identificador del otro hilo */
	    int otro = (idHilo + 1) % 2;
			int local = 0;

			if(idHilo == 0)
				local = turno[otro];
			else
				local = 1 - turno[otro];

	    turno[idHilo] = local;

	    if(idHilo == 0)
	      while(flag[otro] == 1 && local == turno[otro]);
	    else
	      while(flag[otro] == 1 && local != turno[otro]);

			//indica que la siguiente seccion se realiza con acceso exclusivo
			#pragma omp critical
			{
				printf("El hilo %d entro a su seccion critica\n", idHilo);
				int sum = 0;
				/* Suma de los primeros 1000 naturales */
				for(int j = 0; j < 1000; j++) { sum += j; }
				printf("El hilo %d ha terminado su seccion critica\n", idHilo);
				flag[idHilo] = 0;
			 }
		 }
	}//fin de seccion paralela
}
