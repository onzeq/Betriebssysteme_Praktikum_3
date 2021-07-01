/* Erzeuger-Verbraucher-Problem: Verbraucher */

#include "erzeugerverbraucher.h"


int main() {

  int i;                            /* Schleifenzaehler */
  
  int semid;                        /* Nummer der Semaphorgruppe */
  unsigned short init_array[3];     /* Anfangswerte der Semaphore */
  
  /* P- und V-Operationen */
  struct sembuf voll_p, voll_v;
  struct sembuf leer_p, leer_v;
  struct sembuf mutex_p, mutex_v;

  int shmid;
  
  unsigned int *buffer;

  unsigned int *naechstes_belegt;

  unsigned int daten;

  /* Erzeugung von drei Semaphoren mit dem Schluessel 4711:

     Semaphor 0 (voll)  zaehlt die Anzahl belegter Plaetze,
                        soll bei leerem Puffer blockieren
     Semaphor 1 (leer)  zaehlt die Anzahl freier Plaetze,
                        soll bei vollem Puffer blockieren
     Semaphor 2 (mutex) soll den Pufferzugriff wechselseitig ausschliessen 
 
  */
  
  semid = semget(4711, 3, IPC_CREAT | 0777);

  /* Initialisierung der Semaphoren */
  init_array[VOLL] = 0;
  init_array[LEER] = PUFFERKAP;
  init_array[MUTEX] = 1;

  semctl(semid, 0, SETALL, init_array);

  /* voll: Vorbereitung der P- und V-Operationen */
  voll_p.sem_num = VOLL;
  voll_v.sem_num = VOLL;
  voll_p.sem_op  = -1;
  voll_v.sem_op  = 1;
  voll_p.sem_flg = 0;
  voll_v.sem_flg = 0;

  /* leer: Vorbereitung der P- und V-Operationen */
  leer_p.sem_num = LEER;
  leer_v.sem_num = LEER;
  leer_p.sem_op  = -1;
  leer_v.sem_op  = 1;
  leer_p.sem_flg = 0;
  leer_v.sem_flg = 0;
 
  /* mutex: Vorbereitung der P- und V-Operationen */
  mutex_p.sem_num = MUTEX;
  mutex_v.sem_num = MUTEX;
  mutex_p.sem_op  = -1;
  mutex_v.sem_op  = 1;
  mutex_p.sem_flg = 0;
  mutex_v.sem_flg = 0;
    
  shmid = shmget(2019, (PUFFERKAP + 2) * sizeof(unsigned int), 0777 | IPC_CREAT);

  buffer = (unsigned int *)shmat (shmid, 0, 0);

  naechstes_belegt = buffer + PUFFERKAP + 1;

  *naechstes_belegt = 0u;

  srand((unsigned)time(NULL));

  sleep(10);
  
  /* Verbrauchsvorgaenge in der gewuenschten Anzahl */

  for (i = 0; i < ANZAHL_RUNDEN; i++) {

    semop(semid, &voll_p, 1);

    semop(semid, &mutex_p, 1);
	
    /* Daten aus Puffer lesen */
    printf("Verbraucher liest\n");

    daten = buffer[(*naechstes_belegt)++] ;

    *naechstes_belegt %= PUFFERKAP;

    sleep((unsigned int)rand() % 2 + 1);

    semop(semid, &mutex_v, 1);
    
    semop(semid, &leer_v, 1);

    /* Daten verbrauchen */
    
    printf("Verbraucher verbraucht %u\n", daten);

    sleep((unsigned int)rand() % 2 + 1);

  }

  /* Loeschen der Semaphoren */
  semctl(semid, 0, IPC_RMID,0);

  shmdt(buffer);

  shmctl(shmid, IPC_RMID, NULL);

  exit(0);

}

