#include "praktikum3.h"

int main(){

    int count;                            /* Schleifenzaehler */
  
    int ps_semid, p1_semid, p2_semid;       /* Nummer der Semaphorgruppe */
    unsigned short init_array[3];     /* Anfangswerte der Semaphore */
    
    /* P- und V-Operationen */
    //Anwender
    struct sembuf ps_full_p, ps_full_v;
    struct sembuf ps_empty_p, ps_empty_v;
    struct sembuf ps_mutex_p, ps_mutex_v;
    int ps_shmid;
    unsigned int *ps_buffer;
    unsigned int *next_used;

    //Drucker 1
    struct sembuf p1_full_p, p1_full_v;
    struct sembuf p1_empty_p, p1_empty_v;
    struct sembuf p1_mutex_p, p1_mutex_v;
    int p1_shmid;
    unsigned int *p1_buffer;
    unsigned int *p1_next_free;
    
    //Drucker 2
    struct sembuf p2_full_p, p2_full_v;
    struct sembuf p2_empty_p, p2_empty_v;
    struct sembuf p2_mutex_p, p2_mutex_v;
    int p2_shmid;
    unsigned int *p2_buffer;
    unsigned int *p2_next_free;



    unsigned int daten;

    ps_semid = semget(SEM_KEY_SPOOLER, 3, IPC_CREAT | 0777);

    p1_semid = semget(SEM_KEY_PRINTER1, 3, 0777);
    if (p1_semid == -1){
        printf("Fehler: Semaphorgruppe existiert nicht! (Drucker 1)");
        exit(-1);
    }

    p2_semid = semget(SEM_KEY_PRINTER2, 3, 0777);
    if (p2_semid == -1){
        printf("Fehler: Semaphorgruppe existiert nicht! (Drucker 2)");
        exit(-1);
    }

    /* Initialisierung der Semaphoren */
    init_array[VOLL] = 0;
    init_array[LEER] = SPOOLER_BUFFER;
    init_array[MUTEX] = 1;

    semctl(ps_semid, 0, SETALL, init_array);

    /* voll: Vorbereitung der P- und V-Operationen */
    ps_full_p.sem_num = VOLL;
    ps_full_v.sem_num = VOLL;
    ps_full_p.sem_op  = -1;
    ps_full_v.sem_op  = 1;
    ps_full_p.sem_flg = 0;
    ps_full_v.sem_flg = 0;

    /* leer: Vorbereitung der P- und V-Operationen */
    ps_empty_p.sem_num = LEER;
    ps_empty_v.sem_num = LEER;
    ps_empty_p.sem_op  = -1;
    ps_empty_v.sem_op  = 1;
    ps_empty_p.sem_flg = 0;
    ps_empty_v.sem_flg = 0;
    
    /* mutex: Vorbereitung der P- und V-Operationen */
    ps_mutex_p.sem_num = MUTEX;
    ps_mutex_v.sem_num = MUTEX;
    ps_mutex_p.sem_op  = -1;
    ps_mutex_v.sem_op  = 1;
    ps_mutex_p.sem_flg = 0;
    ps_mutex_v.sem_flg = 0;

    //***************DRUCKER 1*************
    /* voll: Vorbereitung der P- und V-Operationen */
    p1_full_p.sem_num = VOLL;
    p1_full_v.sem_num = VOLL;
    p1_full_p.sem_op  = -1;
    p1_full_v.sem_op  = 1;
    p1_full_p.sem_flg = 0;
    p1_full_v.sem_flg = 0;

    /* leer: Vorbereitung der P- und V-Operationen */
    p1_empty_p.sem_num = LEER;
    p1_empty_v.sem_num = LEER;
    p1_empty_p.sem_op  = -1;
    p1_empty_v.sem_op  = 1;
    p1_empty_p.sem_flg = 0;
    p1_empty_v.sem_flg = 0;
    
    /* mutex: Vorbereitung der P- und V-Operationen */
    p1_mutex_p.sem_num = MUTEX;
    p1_mutex_v.sem_num = MUTEX;
    p1_mutex_p.sem_op  = -1;
    p1_mutex_v.sem_op  = 1;
    p1_mutex_p.sem_flg = 0;
    p1_mutex_v.sem_flg = 0;

    //***************DRUCKER 2*************
    /* voll: Vorbereitung der P- und V-Operationen */
    p2_full_p.sem_num = VOLL;
    p2_full_v.sem_num = VOLL;
    p2_full_p.sem_op  = -1;
    p2_full_v.sem_op  = 1;
    p2_full_p.sem_flg = 0;
    p2_full_v.sem_flg = 0;

    /* leer: Vorbereitung der P- und V-Operationen */
    p2_empty_p.sem_num = LEER;
    p2_empty_v.sem_num = LEER;
    p2_empty_p.sem_op  = -1;
    p2_empty_v.sem_op  = 1;
    p2_empty_p.sem_flg = 0;
    p2_empty_v.sem_flg = 0;
    
    /* mutex: Vorbereitung der P- und V-Operationen */
    p2_mutex_p.sem_num = MUTEX;
    p2_mutex_v.sem_num = MUTEX;
    p2_mutex_p.sem_op  = -1;
    p2_mutex_v.sem_op  = 1;
    p2_mutex_p.sem_flg = 0;
    p2_mutex_v.sem_flg = 0;


    ps_shmid = shmget(SHM_KEY_SPOOLER, (SEM_KEY_SPOOLER + 2) * sizeof(unsigned int), 0777 | IPC_CREAT);

    p1_shmid = shmget(SHM_KEY_PRINTER1, (PRINTER_BUFFER + 2) * sizeof(unsigned int), 0777);
    p2_shmid = shmget(SHM_KEY_PRINTER2, (PRINTER_BUFFER + 2) * sizeof(unsigned int), 0777);

    semop(p1_semid, &p1_mutex_p, 1);
    p1_buffer = (unsigned int *)shmat (p1_shmid, 0, 0);
    p1_next_free = p1_buffer + PRINTER_BUFFER;
    *p1_next_free = 0u;
    semop(p1_semid, &p1_mutex_v, 1);

    semop(p2_semid, &p2_mutex_p, 1);
    p2_buffer = (unsigned int *)shmat (p2_shmid, 0, 0);
    p2_next_free = p2_buffer + PRINTER_BUFFER;
    *p2_next_free = 0u;
    semop(p2_semid, &p2_mutex_v, 1);


    ps_buffer = (unsigned int *)shmat (ps_shmid, 0, 0);

    semop(ps_semid, &ps_mutex_p, 1);
    next_used = ps_buffer + SHM_KEY_SPOOLER + 1;
    *next_used = 0u;
    semop(ps_semid, &ps_mutex_v, 1);

    srand((unsigned)time(NULL));

  
  /* Verbrauchsvorgaenge in der gewuenschten Anzahl */

    for (count = 0; count < NUMBER_OF_APPLICATIONS; count++) {

        semop(ps_semid, &ps_full_p, 1);

        semop(ps_semid, &ps_mutex_p, 1);
        
        /* Daten aus Puffer lesen */
        printf("Spooler liest mit Index %d\n", *next_used);

        daten = ps_buffer[(*next_used)++] ;

        *next_used %= SPOOLER_BUFFER;

        sleep((unsigned int)rand() % 2 + 1);

        semop(ps_semid, &ps_mutex_v, 1);
        
        semop(ps_semid, &ps_empty_v, 1);

        /* Daten verbrauchen */
        
        if(count % 2){
            semop(p1_semid, &p1_empty_p, 1);
            semop(p1_semid, &p1_mutex_p, 1);

            printf("Druckauftrag wird an Drucker 1 gesendet Daten: %d\n", daten);

            p1_buffer[(*p1_next_free)] = daten;

            *p1_next_free = *p1_next_free + 1;
            *p1_next_free %= PRINTER_BUFFER;

            semop(p1_semid, &p1_mutex_v, 1);
            semop(p1_semid, &p1_full_v, 1);
        } else
        {
            semop(p2_semid, &p2_empty_p, 1);
            semop(p2_semid, &p2_mutex_p, 1);

            printf("Druckauftrag wird an Drucker 2 gesendet Daten: %d\n", daten);

            p2_buffer[(*p2_next_free)] = daten;

            *p2_next_free = *p2_next_free + 1;
            *p2_next_free %= PRINTER_BUFFER;

            semop(p2_semid, &p2_mutex_v, 1);
            semop(p2_semid, &p2_full_v, 1);
        }
        
    }
    semctl(ps_semid, 0, IPC_RMID,0);

    shmdt(ps_buffer);

    shmctl(ps_shmid, IPC_RMID, NULL);

    exit(0);
}