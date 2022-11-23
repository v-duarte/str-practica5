//Bibliotecas Standards
#include <stdio.h>
#include <stdlib.h>

//Bibliotecas de Pthreads
#include <pthread.h>
#include <sched.h>

//Biblioteca de Semaforos
#include <semaphore.h>

//Bibliotecas de Pipe
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

//Prioridades del Scheduler FIFO
#define max_priority sched_get_priority_max(SCHED_FIFO) //es 99
#define min_priority sched_get_priority_min(SCHED_FIFO) //es 1

//Creo los parametros de los scheduler que van a manejar los hilos
struct sched_param sp1;
struct sched_param sp2;

//Declaro el semaforo
sem_t semaforo;

//Declaro el buffer que almacena las temperaturas
int buffer[3]={0, 0, 0};

//Declaro una variable para detectar el fin de comunicacion
int fin = 0;

void almacenarTemperatura(int temp){
    static int pos=0;
    buffer[pos]=temp;
    pos++;
    if(pos==3)
        pos=0;
}


void * function_thread1(void * var){
    int fd,valorRecibido;
    char * myfifo = "ej2fifo";

    while(!fin){	//Loop hasta que se termina la comunicacion
        fd = open(myfifo, O_RDONLY);//Abro el pipe para leer los valores de temperatura
        read(fd, &valorRecibido, sizeof(int));//Leo la temperatura
        almacenarTemperatura(valorRecibido);//Almaceno la temperatura

        //Control de temperatura
        if(valorRecibido>90)
            printf("La temperatura actual de %d °C excede los 90°C \n",valorRecibido);
        else{
            sem_post(&semaforo);//libera el semaforo es un V
        }
		if (fd < 0)
			fin = 1;
    }
	pthread_exit(NULL);	//Termina el thread
    return NULL;
}

void * function_thread2(void * var){
    while(!fin){	//Loop hasta que se termina la comunicacion
        sem_wait(&semaforo);//pide el semaforo
        int i, promedio=0;
        for(i=0;i<3;i++){
            promedio+=buffer[i];
        }
        printf("El promedio de las temperaturas leidas es de: %d °C \n",promedio/3);
    }
	pthread_exit(NULL);	//Termina el thread
    return NULL;
}

int main()
{
    printf("Inicio\n");

    //Semaforo
    sem_init(&semaforo,0,0);

    //Declaro los Threads
    pthread_t thread1; //Variables del thread1
    pthread_t thread2; //Variables del thread2

    //Creo los atributos para setear las prioridades de los threads
    pthread_attr_t tattr1;
    pthread_attr_t tattr2;
    //Les cargo los valores por defecto
    pthread_attr_init(&tattr1);
    pthread_attr_init(&tattr2);
    //Cargo los parametros de los scheduler en los atributos de los threads y les asigno la politica de scheduling
    sp1.sched_priority=max_priority;
    sp2.sched_priority=min_priority;
    
    pthread_attr_setschedparam(&tattr1,&sp1);
    pthread_attr_setschedparam(&tattr2,&sp2);  
    pthread_attr_setschedpolicy(&tattr1,SCHED_FIFO);
    pthread_attr_setschedpolicy(&tattr2,SCHED_FIFO);

    //Creo los threads
    pthread_create(&thread1,&tattr1,function_thread1,NULL); //creo el thread1
    pthread_create(&thread2,&tattr2,function_thread2,NULL); //creo el thread2
    pthread_join(thread1,NULL); //Espera al que el thread1 finalice
    pthread_join(thread2,NULL); //Espera al que el thread2 finalice
    printf("Fin\n");
    return 0;
}
