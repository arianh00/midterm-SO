#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler_tecnico();
void handler_encargado();
void handler_asistentes();

int main (int argc, char *argv[]) {
    srand(1);
    pid_t tecnico;
    pid_t encargado;
    if(argc <= 1){
        exit(-1);
    }
    int numAsistentes = atoi(argv[1]);
    pid_t asistentes[numAsistentes];

    for (size_t i = 0; i < numAsistentes+2; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error en el fork");
        }
        else if(pid == 0)
        {
            switch (i)
            {
                case 0:
                    if(signal(SIGUSR1, handler_tecnico) == SIG_ERR){
                        perror("Llamada a signal fallida.");
                        exit(-1);
                    }
                    break;
                case 1:
                    if(signal(SIGUSR1, handler_encargado) == SIG_ERR){
                        perror("Llamada a signal fallida.");
                        exit(-1);
                    }
                    break;
                default:
                    if(signal(SIGUSR2, handler_asistentes) == SIG_ERR){
                        perror("Llamada a signal fallida.");
                        exit(-1);
                    }
                    break;
            }
            pause();
        }else{
            switch (i)
            {
                case 0:
                    tecnico = pid;
                    break;
                case 1:
                    encargado = pid;
                    break;
                default:
                    asistentes[i-2] = pid;
                    break;
            }
        }
        
    }
}

void handler_tecnico(){
    if(signal(SIGUSR1, handler_tecnico) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
}

void handler_encargado(){
    if(signal(SIGUSR1, handler_encargado) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
}

void handler_asistentes(){
    if(signal(SIGUSR2, handler_asistentes) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
}