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
    srand(2);
    pid_t tecnico;
    pid_t encargado;
    if(argc <= 1){
        exit(-1);
    }
    int numAsistentes = atoi(argv[1]);
    pid_t asistentes[numAsistentes];

    for (size_t i = 0; i < numAsistentes+2; i++)
    {
        pid_t pid;
        if ((pid = fork()) == -1)
        {
            perror("Error en el fork");
        }
        else if(pid == 0)
        {
            switch (i)
            {
                case 0:
                    printf("Tecnico creado\n");
                    if(signal(SIGUSR1, handler_tecnico) == SIG_ERR){
                        perror("Llamada a signal fallida.");
                        exit(-1);
                    }
                    break;
                case 1:
                    printf("Encargado creado\n");
                    if(signal(SIGUSR1, handler_encargado) == SIG_ERR){
                        perror("Llamada a signal fallida.");
                        exit(-1);
                    }
                    break;
                default:
                    printf("Asistente %d creado\n", (int)i);
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

    int estado_tecnico, estado_encargado;
    
    sleep(2);
    
    if(kill(tecnico, SIGUSR1) != 0){
        perror("Lanzamiento de señal SIGUSR1 al tecnico ha fallado.");
        // TODO matar hijos?
        exit(-1);
    }
    
    waitpid(tecnico, &estado_tecnico, 0);
    
    if(WEXITSTATUS(estado_tecnico) == 0){
        printf("El vuelo no es viable\n");
        kill(encargado, SIGKILL);
        for (size_t i = 0; i < numAsistentes; i++)
        {
            kill(asistentes[i], SIGKILL);
        }
        exit(0);
    }
    
    if(kill(encargado, SIGUSR1) != 0){
        perror("Lanzamiento de señal SIGUSR1 al encargado ha fallado.");
        exit(-1);
    }
    
    waitpid(encargado, &estado_encargado, 0);
    
    int pasajeros = 0;
    
    for (size_t i = 0; i < numAsistentes; i++)
    {
        if(kill(asistentes[i], SIGUSR2) != 0){
            perror("Lanzamiento de señal SIGUSR2 a un asistente ha fallado.");
            exit(-1);
        }
    }
    
    int pasajeros_embarcados = 0;
    for (size_t i = 0; i < numAsistentes; i++)
    {
        wait(&pasajeros_embarcados);
        pasajeros += WEXITSTATUS(pasajeros_embarcados);
    }
    
    if(WEXITSTATUS(estado_encargado) == 1){
        pasajeros -= 10;
    }
    printf("Pasajeros abordo: %d\n", pasajeros);
}

void handler_tecnico(){
    if(signal(SIGUSR1, handler_tecnico) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
    printf("Se ha recibido la señal, comprobando si el vuelo es viable.\n");
    int tiempo_sueno = (rand() % 4) + 3;
    sleep(tiempo_sueno);
    int viabilidad;
    if((viabilidad = (rand() % 2)) == 0){
        exit(viabilidad);
    }
    printf("El vuelo es viable.\n");
    exit(viabilidad);
}

void handler_encargado(){
    if(signal(SIGUSR1, handler_encargado) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
    printf("Se ha recibido la señal, comprobando si hay overbooking.\n");
    int overbooking = rand() % 2;
    sleep(2);
    if(overbooking == 0){
        printf("No hay overbooking.\n");
        exit(overbooking);
    }
    printf("Hay overbooking.\n");
    exit(overbooking);
}

void handler_asistentes(){
    if(signal(SIGUSR2, handler_asistentes) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
    printf("Se ha recibido la señal, procediendo con un embarque.\n");
    int tiempo_embarque = (rand() % 4) + 3;
    sleep(tiempo_embarque);
    printf("El embarque ha durado %ds.\n", tiempo_embarque);
    int embarcados = (rand() % 11) + 20;
    printf("Se han embarcado %d personas.\n", embarcados);
    exit(embarcados);
}