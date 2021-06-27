#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ANZAHLANWENDUNGEN 30


#define SPOOLERPUFFER 5
#define DRUCKERPUFFER 1

#define SEMKEYSPOOLER 4771
#define SHMKEYSPOOLER 2019

#define SEMKEYDRUCKER1 4775
#define SHMKEYDRUCKER1 2020

#define SEMKEYDRUCKER2 4776
#define SHMKEYDRUCKER2 2021

#define VOLL 0
#define LEER 1
#define MUTEX 2