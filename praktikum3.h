#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NUMBER_OF_APPLICATIONS 30


#define SPOOLER_BUFFER 5 
#define PRINTER_BUFFER 1

// Keys fr shared memory and semaphores spooler
#define SEM_KEY_SPOOLER 4000
#define SHM_KEY_SPOOLER 6000

// Keys fr shared memory and semaphores printer_1
#define SEM_KEY_PRINTER1 4001
#define SHM_KEY_PRINTER1 6001

// Keys fr shared memory and semaphores printer_2
#define SEM_KEY_PRINTER2 4002
#define SHM_KEY_PRINTER2 6002

#define VOLL 0
#define LEER 1
#define MUTEX 2


