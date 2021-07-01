#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define PUFFERKAP      3     /* Kapazitaet des Puffers                          */
#define ANZAHL_RUNDEN 10     /* Anzahl der Erzeugungs- bzw. Verbrauchsvorgaenge */
#define VOLL           0     /* Symbolische Namen für die Semaphornummern       */
#define LEER           1     
#define MUTEX          2
