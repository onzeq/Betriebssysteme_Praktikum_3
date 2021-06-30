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

#define SEM_KEY_SPOOLER 4771
#define SHM_KEY_SPOOLER 2019

#define SEM_KEY_PRINTER1 4775
#define SHM_KEY_PRINTER1 2020

#define SEM_KEY_PRINTER2 4776
#define SHM_KEY_PRINTER2 2021

#define VOLL 0
#define LEER 1
#define MUTEX 2