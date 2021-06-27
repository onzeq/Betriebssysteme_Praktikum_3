#include "praktikum3.h"

int main(){
    int counter;                            /* Schleifenzaehler */
  
    int p_semid;                        /* Nummer der Semaphorgruppe */
    unsigned short init_array[3];     /* Anfangswerte der Semaphore */
    
    /* P- und V-Operationen */
    struct sembuf p_full_p, p_full_v;
    struct sembuf p_empty_p, p_empty_v;
    struct sembuf p_mutex_p, p_mutex_v;

    int p_shmid;
    
    unsigned int *p_buffer;

    unsigned int *p_next_used;

    unsigned int data;

    p_semid = semget(SEMKEYDRUCKER1, 3, IPC_CREAT | 0777);

    /* Initialisierung der Semaphoren */
    init_array[VOLL] = 0;
    init_array[LEER] = DRUCKERPUFFER;
    init_array[MUTEX] = 1;

    semctl(p_semid, 0, SETALL, init_array);

    /* voll: Vorbereitung der P- und V-Operationen */
    p_full_p.sem_num = VOLL;
    p_full_v.sem_num = VOLL;
    p_full_p.sem_op  = -1;
    p_full_v.sem_op  = 1;
    p_full_p.sem_flg = 0;
    p_full_v.sem_flg = 0;

    /* leer: Vorbereitung der P- und V-Operationen */
    p_empty_p.sem_num = LEER;
    p_empty_v.sem_num = LEER;
    p_empty_p.sem_op  = -1;
    p_empty_v.sem_op  = 1;
    p_empty_p.sem_flg = 0;
    p_empty_v.sem_flg = 0;
    
    /* mutex: Vorbereitung der P- und V-Operationen */
    p_mutex_p.sem_num = MUTEX;
    p_mutex_v.sem_num = MUTEX;
    p_mutex_p.sem_op  = -1;
    p_mutex_v.sem_op  = 1;
    p_mutex_p.sem_flg = 0;
    p_mutex_v.sem_flg = 0;

    p_shmid = shmget(SHMKEYDRUCKER1, (SEMKEYDRUCKER1 + 2) * sizeof(unsigned int), 0777 | IPC_CREAT);

    p_buffer = (unsigned int *)shmat (p_shmid, 0, 0);
    semop(p_semid, &p_mutex_p, 1);
    p_next_used = p_buffer + SEMKEYDRUCKER1 + 1;
    *p_next_used = 0u;
    semop(p_semid, &p_mutex_v, 1);

    srand((unsigned)time(NULL));

    for (counter = 0; counter < ANZAHLANWENDUNGEN/2; counter++) {
        semop(p_semid, &p_full_p, 1);

        semop(p_semid, &p_mutex_p, 1);
        
        /* Daten aus Puffer lesen */
        //printf("Drucker liest mit Index %d\n", *p_next_used);

        data = p_buffer[(*p_next_used)++] ;

        *p_next_used %= DRUCKERPUFFER;

        printf("Drucker 1 beginnt Drucken von: %d\n", data);
        sleep((unsigned int)rand() % 5 + 2);
        printf("Drucker 1 beendet Drucken von: %d\n\n", data);

        semop(p_semid, &p_mutex_v, 1);
        
        semop(p_semid, &p_empty_v, 1);    
    }

    semctl(p_semid, 0, IPC_RMID,0);

    shmdt(p_buffer);

    shmctl(p_shmid, IPC_RMID, NULL);

    exit(0);

}