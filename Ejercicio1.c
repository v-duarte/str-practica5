#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

void *thread_function(void *args);
double entro[2];
double latencia[2];
double latenciaPromedio[2];
double salio[2];

// Para calcular tiempo
double getTime()
{
	double sec;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	sec = tv.tv_sec + tv.tv_usec / 1000000; // Tiempo en segundos
	return sec;
}

void die()
{
	perror(NULL);
	abort();
}

int main(int argc, char *argv[])
{
	pthread_t threads[2];

	for (int i = 0; i < 2; i++)
		if (pthread_create(threads + i, NULL, thread_function, (void *)(intptr_t)i))
			die();

	for (int i = 0; i < 2; i++)
		if (pthread_join(threads[i], NULL))
			die();

    latenciaPromedio[0] = latencia[0]/1000;
    latenciaPromedio[1] = latencia[1]/1000;
    
	printf("\n");
    printf("Latencia promedio del primer thread: \t%f milisegundos\n", latenciaPromedio[0]);
	printf("\n");
	printf("-----------------------\n");
	printf("\n");
    printf("Latencia promedio del segundo thread: \t%f milisegundos\n", latenciaPromedio[1]);
	printf("\n");

    return 0;
}

void *thread_function(void *args)
{
    int id = (intptr_t)args;

    for(int i = 0; i < 1000; i++)
    {	
    	entro[id]=getTime();// toma el tiempo en el que entro el thread 
       	usleep(10000);// lo duerme por 10 microsegundos 
       	salio[id]=getTime();// toma el tiempo en el que salio el thread 


        latencia[id] = latencia[id] + (salio[id] - entro[id] + 10/1000); //valor de latencia acumulada en segundos
        
        //latencia del thread es igual ala latencia + (el tiempo que salio el thread - el tiempo que entro - 10 ms)
        // porque 10 ms porque se ejecutan en 1000 opotunidades , y como se ejecutan 100 veces , cada uno va ser 10 ms 		
    }

    return NULL;
}
