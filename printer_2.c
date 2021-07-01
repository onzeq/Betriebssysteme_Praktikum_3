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

    p_semid = semget(SEM_KEY_PRINTER2, 3, IPC_CREAT | 0777);

    /* Initialisierung der Semaphoren */
    init_array[VOLL] = 0;
    init_array[LEER] = PRINTER_BUFFER;
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

    p_shmid = shmget(SHM_KEY_PRINTER2, (SEM_KEY_PRINTER2 + 2) * sizeof(unsigned int), 0777 | IPC_CREAT);

    p_buffer = (unsigned int *)shmat (p_shmid, 0, 0);
    semop(p_semid, &p_mutex_p, 1);
    p_next_used = p_buffer + SEM_KEY_PRINTER2 + 1;
    *p_next_used = 0u;
    semop(p_semid, &p_mutex_v, 1);

    srand((unsigned)time(NULL));

    for (counter = 0; counter < NUMBER_OF_APPLICATIONS/2; counter++) {
        semop(p_semid, &p_full_p, 1);

        semop(p_semid, &p_mutex_p, 1);
        
        /* //read data from buffer
        printf("printer 2 reads with index: %d\n", *p_next_used); */

        data = p_buffer[(*p_next_used)++] ;

        *p_next_used %= PRINTER_BUFFER;

        printf("printer 2 starts printing: %d\n", data);
        sleep((unsigned int)rand() % 5 + 2);
        printf("printer 2 starts printing: %d\n\n", data);

        semop(p_semid, &p_mutex_v, 1);
        
        semop(p_semid, &p_empty_v, 1);    
    }

    semctl(p_semid, 0, IPC_RMID,0); // remove semaphore group

    shmdt(p_buffer); // deactivate shared memory

    shmctl(p_shmid, IPC_RMID, NULL); // remove shared memory segment

    exit(0);

}