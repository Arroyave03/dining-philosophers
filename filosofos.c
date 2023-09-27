/**
 * Dining philosophers problem
 *
 * @author Samuel Arroyave Romero
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

/*Se define la cantidad de filosofos, la cantidad de tenedores
que se utilizarán en el problema y los estados*/

#define N 5
#define IZQUIERDA (i + N - 1) % N
#define DERECHA (i + 1) % N
#define PENSANDO 0
#define HAMBRIENTO 1
#define COMIENDO 2

/*Se definen los semaforos
mutex: Semaforo binario para evitar que dos filósofos cambien su estado al mismo tiempo
s[N]: Semaforo que se utiliza para indicar si el filosofo[i] puede comer o no*/

sem_t mutex;
sem_t s[N];

/*Se define el estado de los filosofos y los tenedores*/

int estado[N];
int tenedores[N];

/*función que se encarga de verificar si el filosofo puede comer o no*/

void probar(int i)
{
    if (estado[i] == HAMBRIENTO && estado[IZQUIERDA] != COMIENDO && estado[DERECHA] != COMIENDO)
    {
        estado[i] = COMIENDO;
        sem_post(&s[i]);
    }
}

/*Función que se encarga de usar los tenedores*/

void usar_tenedores(int i)
{
    sem_wait(&mutex);
    estado[i] = HAMBRIENTO;
    probar(i);
    sem_post(&mutex);
    sem_wait(&s[i]);
}

/*Función que se encarga de devolver los tenedores*/

void devolver_tenedores(int i)
{
    sem_wait(&mutex);
    estado[i] = PENSANDO;
    probar(IZQUIERDA);
    probar(DERECHA);
    sem_post(&mutex);
}

/*Función que se encarga de simular el comportamiento de los filosofos*/

void *filosofo(void *arg)
{
    int i = *(int *)arg;
    while (1)
    {
        printf("Filosofo[%d] pensando...\n", i);
        usar_tenedores(i);
        printf("Filosofo[%d] usa tenedores (%d,%d)\n", i, i, DERECHA);
        printf("Filosofo[%d] comiendo...\n", i);
        sleep(1);
        devolver_tenedores(i);
        printf("Filosofo[%d] devuelve tenedores (%d,%d)\n", i, i, DERECHA);
    }
}

int main()
{
    int i;
    pthread_t filosofos[N];
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
    {
        sem_init(&s[i], 0, 0);
    }
    for (i = 0; i < N; i++)
    {
        pthread_create(&filosofos[i], NULL, &filosofo, &i);
    }
    for (i = 0; i < N; i++)
    {
        pthread_join(filosofos[i], NULL);
    }
    sem_destroy(&mutex);
    for (i = 0; i < N; i++)
    {
        sem_destroy(&s[i]);
    }
    return 0;
}
