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
    pid_t tecnico;
    pid_t encargado;
    // Compruebo que solo haya un argumento
    if(argc != 2){
        printf("No se han recibido unos argumentos adecuados\n");
        exit(-1);
    }

    // Convierto el número recibido por argumentos en un entero
    int numAsistentes = atoi(argv[1]);

    // Compruebo que asistentes no sea 0
    // Esto se puede dar o bien porque ha recibido un cero por linea de comandos
    // O bien porque atoi no ha podido convertir el string(por default devuelve 0)
    if (numAsistentes == 0)
    {
        printf("No se pudo establecer correctamente el número de asistentes\n");
        exit(-1);
    }
    
    pid_t* asistentes = (pid_t*)malloc(sizeof(pid_t)*numAsistentes);

    // Creo todos los procesos en un for
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
                // Asigno a cada proceso su tratamiento de señal
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
                // Si estamos en una iteración diferente a la 0 o la 1 entonces ya estamos creando asistentes
                // De ahí que se use el default para el switch
                default:
                    printf("Asistente %d creado\n", (int)(i-1));
                    if(signal(SIGUSR2, handler_asistentes) == SIG_ERR){
                        perror("Llamada a signal fallida.");
                        exit(-1);
                    }
                    break;
            }
            pause();
        }else{
            // Guardo los pids para posteriormente poder hacer las llamada a kills
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
    
    // Se manda la señal SIGUSR1 al tecnico hasta que la llamada no falle
    while(kill(tecnico, SIGUSR1) != 0){
        perror("Lanzamiento de señal SIGUSR1 al tecnico ha fallado.");
    }
    
    // Esperamos a que el técnico termine
    waitpid(tecnico, &estado_tecnico, 0);
    
    // Si el vuelo no es viable (exit con 0), entonces matamos los procesos y salimos
    // Se sale con 0 dado que no ha ocurrido ningún error
    if (WIFEXITED(estado_tecnico)){
        if(WEXITSTATUS(estado_tecnico) == 0){
            printf("El vuelo no es viable\n");
            kill(encargado, SIGKILL);
            for (size_t i = 0; i < numAsistentes; i++)
            {
                kill(asistentes[i], SIGKILL);
            }
            exit(0);
        }
    }else{
        perror("Tecnico terminado por señal externa.");
        exit(-1);
    }
    
    
    // Se manda la señal SIGUSR1 al encargado hasta que la llamada no falle
    while(kill(encargado, SIGUSR1) != 0){
        perror("Lanzamiento de señal SIGUSR1 al encargado ha fallado.");
    }
    
    // Esperamos a que el encargado termine
    waitpid(encargado, &estado_encargado, 0);
    
    if(!WIFEXITED(estado_encargado)){
        perror("Encargado terminado por señal externa.");
        exit(-1);
    }

    int pasajeros = 0;
    
    for (size_t i = 0; i < numAsistentes; i++)
    {
        // Mandamos la señal SIGUSR2 a todos los asistentes hasta que ninguna falle
        while(kill(asistentes[i], SIGUSR2) != 0){
            perror("Lanzamiento de señal SIGUSR2 a un asistente ha fallado.");
        }
    }
    
    int pasajeros_embarcados = 0;
    for (size_t i = 0; i < numAsistentes; i++)
    {
        // Esperamos a que cualquiera de los asistentes termine y añadimos los pasajeros al total
        wait(&pasajeros_embarcados);
        if(!WIFEXITED(pasajeros_embarcados)){
            perror("Un asistente ha terminado por señal externa");
            exit(-1);
        }
        pasajeros += WEXITSTATUS(pasajeros_embarcados);
    }
    
    // Si había overbooking entonces restamos 10 al total de pasajeros
    if(WEXITSTATUS(estado_encargado) == 1){
        pasajeros -= 10;
    }
    printf("Pasajeros abordo: %d\n", pasajeros);
    free(asistentes);
}

// A todos los handler se les ha dado su propia semilla de random para que no se generen repeticiiones
// Esto se ha hecho estableciendo los pids como semillas
void handler_tecnico(){
    if(signal(SIGUSR1, handler_tecnico) == SIG_ERR){
        perror("Llamada a signal fallida.");
        exit(-1);
    }
    srand(getpid());
    printf("Se ha recibido la señal, comprobando si el vuelo es viable.\n");
    // Se duerme un tiempo aleatorio entre 3 y 6
    int tiempo_sueno = (rand() % 4) + 3;
    sleep(tiempo_sueno);
    int viabilidad;
    // Si el vuelo es viable lo especifica, y sino simplemente sale con 0
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
    srand(getpid());
    printf("Se ha recibido la señal, comprobando si hay overbooking.\n");
    int overbooking = rand() % 2;
    sleep(2);
    // Imprime un mensaje dependiendo de si hay overbooking o no
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
    srand(getpid());
    printf("Se ha recibido la señal, procediendo con un embarque.\n");
    // Se duerme un numero aleatorio entre 3 y 6
    int tiempo_embarque = (rand() % 4) + 3;
    sleep(tiempo_embarque);
    printf("El embarque ha durado %ds.\n", tiempo_embarque);
    // Embarca entre 20 y 30 personas
    int embarcados = (rand() % 11) + 20;
    printf("Se han embarcado %d personas.\n", embarcados);
    exit(embarcados);
}