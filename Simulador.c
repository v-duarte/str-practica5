//Bibliotecas Standard
#include <stdio.h>
#include <stdlib.h>

//Bibliotecas de Pipe
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
 
int main()
{
    int fd;
	char * pipe = "ej2fifo";
    mkfifo(pipe, 0777);
    fd = open(pipe, O_RDWR);
    
    FILE *fp;
    fp = fopen("datos.txt", "r"); // read mode

    int tiempo;
    int temp;
    while(!feof(fp))
    {
        fscanf(fp, "%d\t%d\n", &tiempo, &temp);
        sleep(tiempo);
        printf("%d\t%d\n", tiempo, temp);
        write(fd, &temp, sizeof(temp));
    }

    fclose(fp);
    close(fd);
    unlink(pipe);

    return 0;
}

