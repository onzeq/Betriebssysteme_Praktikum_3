#include "praktikum3.h"

int main(){
    int counter;   //Schleifenzaehler
  
    int p_semid;                //number of semaphore group
    unsigned short init_array[3];     // array to initialize semaphore group
    
    // declaration semaphors
    struct sembuf p_full_p, p_full_v;
    struct sembuf p_empty_p, p_empty_v;
    struct sembuf p_mutex_p, p_mutex_v;

    int p_shmid;
    
    unsigned int *p_buffer;

    unsigned int *p_next_used;

    unsigned int data;

    p_semid = semget(SEM_KEY_PRINTER1, 3, IPC_CREAT | 0777);

    // init array for semaphor group 
    init_array[VOLL] = 0;
    init_array[LEER] = PRINTER_BUFFER;
    init_array[MUTEX] = 1;

    semctl(p_semid, 0, SETALL, init_array);

    //full semaphore: configuration printer wait (p) and signal (v) operations
    p_full_p.sem_num = VOLL;
    p_full_v.sem_num = VOLL;
    p_full_p.sem_op  = -1;
    p_full_v.sem_op  = 1;
    p_full_p.sem_flg = 0;
    p_full_v.sem_flg = 0;

    //empty semaphore: configuration printer wait (p) and signal (v) operations
    p_empty_p.sem_num = LEER;
    p_empty_v.sem_num = LEER;
    p_empty_p.sem_op  = -1;
    p_empty_v.sem_op  = 1;
    p_empty_p.sem_flg = 0;
    p_empty_v.sem_flg = 0;
    
    //this is a test
    //mutex semaphore: configuration printer wait (p) and signal (v) operations
    p_mutex_p.sem_num = MUTEX;
    p_mutex_v.sem_num = MUTEX;
    p_mutex_p.sem_op  = -1;
    p_mutex_v.sem_op  = 1;
    p_mutex_p.sem_flg = 0;
    p_mutex_v.sem_flg = 0;

    // assign shared memory ID  
    p_shmid = shmget(SHM_KEY_PRINTER1, (SEM_KEY_PRINTER1 + 2) * sizeof(unsigned int), 0777 | IPC_CREAT);

    p_buffer = (unsigned int *)shmat (p_shmid, 0, 0);
    semop(p_semid, &p_mutex_p, 1); //  p_mutex_wait(1)
    
    // mutex wait signal for critical memory action
    p_next_used = p_buffer + SEM_KEY_PRINTER1 + 1;
    *p_next_used = 0u; // set next used to zero
    semop(p_semid, &p_mutex_v, 1); //  p_mutex_signal(1)

    srand((unsigned)time(NULL)); // initialize seed for random 

    for (counter = 0; counter < NUMBER_OF_APPLICATIONS/2; counter++) {
        semop(p_semid, &p_full_p, 1); // p_full_wait(1)

        semop(p_semid, &p_mutex_p, 1); // p_mutex_wait(1)
        
        /* //read data from buffer
        printf("printer 1 reads with index: %d\n", *p_next_used); */

        data = p_buffer[(*p_next_used)++] ; // get data from shared memory buffer

        *p_next_used %= PRINTER_BUFFER; //reset pointer

        printf("printer 1 starts printing: %d\n", data);
        sleep((unsigned int)rand() % 5 + 2);
        printf("printer 1 finishes printing: %d\n\n", data);

        semop(p_semid, &p_mutex_v, 1); // p_mutex_signal(1)
        
        semop(p_semid, &p_empty_v, 1); // p_empty_signal(1)
    }

    semctl(p_semid, 0, IPC_RMID,0); // remove semaphore group

    shmdt(p_buffer); // deactivate shared memory

    shmctl(p_shmid, IPC_RMID, NULL); // remove shared memory segment

    exit(0);

}