/* Erzeuger-Verbraucher-Problem: Erzeuger */

#include "erzeugerverbraucher.h"

int main() {

  int i;                            /* Schleifenzaehler */
  
  int semid;                        /* Nummer der Semaphorgruppe */

  /* P- und V-Operationen */
  struct sembuf voll_p, voll_v;
  struct sembuf leer_p, leer_v;
  struct sembuf mutex_p, mutex_v;

  int shmid;
   
  unsigned int *buffer;

  unsigned int *naechstes_frei;

  unsigned int daten;

  /* Zugriff auf die drei Semaphoren mit dem Schluessel 4711,
     die vom Verbraucher generiert wurden (siehe Kommentar dort) */
  semid = semget(4711, 3, 0777);
  
  if (semid == -1) {
    printf("Fehler: Semaphorgruppe existiert nicht!");
    exit(-1);
  }

  /* Erzeugung von drei Semaphoren mit dem Schluessel 4711:

     Semaphor 0 (voll)  zaehlt die Anzahl belegter Plaetze,
                        soll bei leerem Puffer blockieren
     Semaphor 1 (leer)  zaehlt die Anzahl freier Plaetze,
                        soll bei vollem Puffer blockieren
     Semaphor 2 (mutex) soll den Pufferzugriff wechselseitig ausschliessen 
 
  */

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
    
  shmid = shmget(2019, (PUFFERKAP + 2) * sizeof(unsigned int), 0777);

  buffer = (unsigned int *)shmat (shmid, 0, 0);

  naechstes_frei = buffer + PUFFERKAP;

  *naechstes_frei = 0u;

  srand((unsigned)time(NULL));

  /* Erzeugungsvorgaenge in der gewuenschten Anzahl */

  for (i = 0; i < ANZAHL_RUNDEN; i++) {

    /* Daten erzeugen */

    daten = (unsigned int)rand() % 256;

    printf("Erzeuger erzeugt %u\n", daten);
		
    sleep((unsigned int)rand() % 2 + 1);

    semop(semid, &leer_p, 1);

    semop(semid, &mutex_p, 1);

    /* Daten schreiben */
    printf("Erzeuger schreibt\n");

    buffer[(*naechstes_frei)++] = daten;

    *naechstes_frei %= PUFFERKAP;

    sleep((unsigned int)rand() % 2 + 1);

    semop(semid, &mutex_v, 1);

    semop(semid, &voll_v, 1);
  
  }

  shmdt(buffer);
    
  exit(0);

}
