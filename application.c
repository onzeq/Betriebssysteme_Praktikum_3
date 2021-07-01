#include "praktikum3.h"

int main(){

    int counter; //Schleifenzaehler

    int app_status; // status of died child process 

    int app_semid; //Nummer der Semaphorengruppe

    // Declaration semaphoren
    struct sembuf app_full_p, app_full_v;
    struct sembuf app_empty_p, app_empty_v;
    struct sembuf app_mutex_p, app_mutex_v;

    int app_shmid;

    unsigned int *app_buffer;

    unsigned int *app_next_free;

    unsigned int data;

    app_semid = semget(SEM_KEY_SPOOLER, 3, 0777);
    
    if (app_semid == -1){
        printf("Fehler: Semaphorgruppe existiert nicht! (Anwendung)");
        exit(-1);
    } 

    // full semaphore: configuration application wait (p) and signal (v) operations
    app_full_p.sem_num = VOLL;
    app_full_v.sem_num = VOLL;
    app_full_p.sem_op  = -1;
    app_full_v.sem_op  = 1;
    app_full_p.sem_flg = 0;
    app_full_v.sem_flg = 0;

    // empty semaphore: configuration application wait (p) and signal (v) operations
    app_empty_p.sem_num = LEER;
    app_empty_v.sem_num = LEER;
    app_empty_p.sem_op  = -1;
    app_empty_v.sem_op  = 1;
    app_empty_p.sem_flg = 0;
    app_empty_v.sem_flg = 0;
    
    // mutex semaphore: configuration application wait (p) and signal (v) operations
    app_mutex_p.sem_num = MUTEX;
    app_mutex_v.sem_num = MUTEX;
    app_mutex_p.sem_op  = -1;
    app_mutex_v.sem_op  = 1;
    app_mutex_p.sem_flg = 0;
    app_mutex_v.sem_flg = 0;

    //Zuweisung shared memory ID    
    app_shmid = shmget(SHM_KEY_SPOOLER, (SPOOLER_BUFFER + 2) * sizeof(unsigned int), 0777);

    //inizialisierung des pointers
    semop(app_semid, &app_mutex_p, 1); // app_mutex_wait(1)
    unsigned int *tmp = (unsigned int *)shmat (app_shmid, 0, 0);
    app_next_free = tmp + SPOOLER_BUFFER;
    *app_next_free = 0u;
    semop(app_semid, &app_mutex_v, 1); // app_mutex_signal(1)


    //create applications
    srand((unsigned)time(NULL)); // init seed randomization generator
    
    for (counter = 0; counter < NUMBER_OF_APPLICATIONS; counter++)
    {
        if(fork() == 0){
            printf("Started %d. application\n", counter);
            app_buffer = (unsigned int *)shmat (app_shmid, 0, 0);
            app_next_free = app_buffer + SPOOLER_BUFFER;

            semop(app_semid, &app_empty_p, 1); // app_empty_wait(1)
            semop(app_semid, &app_mutex_p, 1); // app_mutex_wait(1)

            data = counter;
            printf("Application Nr. %d writes commission Nr %d with index %d\n", counter, data, *app_next_free);

            app_buffer[(*app_next_free)] = data;

            *app_next_free = *app_next_free + 1;
            *app_next_free %= SPOOLER_BUFFER;

            semop(app_semid, &app_mutex_v, 1);
            semop(app_semid, &app_full_v, 1);
            exit(0);
        }
        sleep((unsigned int)rand() % 5);
        //sleep((unsigned int)rand() % 2);
    }

    for (counter = 0; counter<NUMBER_OF_APPLICATIONS; counter++){
        wait(&app_status);
    }

    // Freigabe Speicher
    shmdt(app_buffer);

    exit(0);

}