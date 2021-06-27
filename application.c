#include "praktikum3.h"

int main(){

    int counter; //Schleifenzaehler

    int app_status;

    int app_semid; //Nummer der Semaphorengruppe

    struct sembuf app_full_p, app_full_v;
    struct sembuf app_empty_p, app_empty_v;
    struct sembuf app_mutex_p, app_mutex_v;

    int app_shmid;

    unsigned int *app_buffer;

    unsigned int *app_next_free;

    unsigned int data;

    app_semid = semget(SEMKEYSPOOLER, 3, 0777);
    
    if (app_semid == -1){
        printf("Fehler: Semaphorgruppe existiert nicht! (Anwendung)");
        exit(-1);
    }

      /* voll: Vorbereitung der P- und V-Operationen */
    app_full_p.sem_num = VOLL;
    app_full_v.sem_num = VOLL;
    app_full_p.sem_op  = -1;
    app_full_v.sem_op  = 1;
    app_full_p.sem_flg = 0;
    app_full_v.sem_flg = 0;

    /* leer: Vorbereitung der P- und V-Operationen */
    app_empty_p.sem_num = LEER;
    app_empty_v.sem_num = LEER;
    app_empty_p.sem_op  = -1;
    app_empty_v.sem_op  = 1;
    app_empty_p.sem_flg = 0;
    app_empty_v.sem_flg = 0;
    
    /* mutex: Vorbereitung der P- und V-Operationen */
    app_mutex_p.sem_num = MUTEX;
    app_mutex_v.sem_num = MUTEX;
    app_mutex_p.sem_op  = -1;
    app_mutex_v.sem_op  = 1;
    app_mutex_p.sem_flg = 0;
    app_mutex_v.sem_flg = 0;
        
    app_shmid = shmget(SHMKEYSPOOLER, (SPOOLERPUFFER + 2) * sizeof(unsigned int), 0777);

    //inizialisierung des pointers
    semop(app_semid, &app_mutex_p, 1);
    unsigned int *tmp = (unsigned int *)shmat (app_shmid, 0, 0);
    app_next_free = tmp + SPOOLERPUFFER;
    *app_next_free = 0u;
    semop(app_semid, &app_mutex_v, 1);


    //erzeugen der Anwendungen
    srand((unsigned)time(NULL));
    for (counter = 0; counter < ANZAHLANWENDUNGEN; counter++)
    {
        if(fork() == 0){
            printf("%dte Anwendung gestartet\n", counter);
            app_buffer = (unsigned int *)shmat (app_shmid, 0, 0);
            app_next_free = app_buffer + SPOOLERPUFFER;

            semop(app_semid, &app_empty_p, 1);
            semop(app_semid, &app_mutex_p, 1);

            data = counter + 100;
            printf("%dte Anwendung Schreibt Druckauftrag Nr. %d mit Index %d\n", counter, data, *app_next_free);

            app_buffer[(*app_next_free)] = data;

            *app_next_free = *app_next_free + 1;
            *app_next_free %= SPOOLERPUFFER;

            semop(app_semid, &app_mutex_v, 1);
            semop(app_semid, &app_full_v, 1);
            exit(0);
        }
        sleep((unsigned int)rand() % 5);
        //sleep((unsigned int)rand() % 2);
    }

    for (counter = 0; counter<ANZAHLANWENDUNGEN; counter++){
        wait(&app_status);
    }


    shmdt(app_buffer);

    exit(0);

}