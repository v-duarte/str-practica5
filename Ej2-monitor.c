#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

void *mayor(void *args);
void *menor(void *args);
void die();

pthread_mutex_t mutexMayor =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMenor =PTHREAD_MUTEX_INITIALIZER;


int temp;

int main(int argc, char *argv[])
{
    int fd;
    char * pipe = "pipe";

	pthread_t threads[2];

    pthread_mutex_lock(&mutexMenor);

    if (pthread_create(threads + 0, NULL, mayor, (void *)(intptr_t)0))
        die();
    if (pthread_create(threads + 1, NULL, menor, (void *)(intptr_t)1))
        die();

    for(int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutexMayor);
        printf("Ingrese un valor: ");
        scanf("%d", &temp);
        pthread_mutex_unlock(&mutexMayor);
    }

    if (pthread_join(threads[0], NULL))
		die();
    if (pthread_join(threads[1], NULL))
		die();

    pthread_mutex_destroy(&mutexMayor);
    pthread_mutex_destroy(&mutexMenor);

    return 0;
}

void *mayor(void *args)
{
    for(int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutexMayor);

        printf("Se ingreso al thread mayor");

        if(temp > 90)
            printf("\t---------- ALERTA DE SOBRECALENTAMIENTO ----------\n");
        else
            pthread_mutex_unlock(&mutexMenor);
        
        pthread_mutex_unlock(&mutexMayor);
    }
}

void *menor(void *args)
{
    int ultimosValores[3];
    int total;
    float promedio;

    for(int j = 0; j < 10; j++)
    {
        pthread_mutex_lock(&mutexMenor);

        printf("Se ingreso al thread menor");

        total = 0;

    
        ultimosValores[i] = temp;
        
        ultimosValores[i+1] = ultimosValores[i+2];

        ultimosValores[i+2]] =ultimosValores[i-3] 



        total += ultimosValores[i];
            
        

        
        promedio = total / 3.0;
        printf("\tPromedio de los ultimos 3 valores: %.2f", promedio);

    }
}

void die()
{
	perror(NULL);
	abort();
}