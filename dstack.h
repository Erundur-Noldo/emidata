/* my personal dynamically allocated stack library
by Emika
10-11-2024 to 10-11-2024

  ____
 /    \
| _  _ |
|      |
 \    /
  \  /
   \/



this whole library is basically a subset of my dstack
*/



#ifndef DSTACK_H
#define DSTACK_H


#include <stdbool.h>
#include "common.h"
#include "dstack.h"


/*--------------- DEFINES ---------------*/
#define DEFAULT_INITIAL_SIZE 16
#define DEFAULT_GROWTH_EXPONENTIAL 2.0


/*--------------- STRUCTS ---------------*/
typedef struct Dstack {
    int data_size;
    int data_type;
    int size;
    int max_size;
    float growth_exponential;
    char *data;
} Dstack;

/*--------------- ENUMS ---------------*/




/*--------------- CREATION FUNCTIONS ---------------*/
Dstack *emi_dstack_create          (int data_size, int data_type);
Dstack *emi_dstack_createWithParas (int data_size, int data_type, int initial_size, float growth_exponential);
Dstack *emi_dstack_createFromArray (void *data, int array_length, int data_size, int data_type);
Dstack *emi_dstack_createCopy      (Dstack *original);

/*--------------- PEEKING FUNCTIONS ---------------*/
void   *emi_dstack_peek      (Dstack *dstack);
void   *emi_dstack_top       (Dstack *dstack);

/*--------------- POPPING FUNCTIONS ---------------*/
void   *emi_dstack_pop       (Dstack *dstack);
void   *emi_dstack_popRaw    (Dstack *dstack);
void    emi_dstack_popSilent (Dstack *dstack);

/*--------------- PUSHING FUNCTIONS ---------------*/
void    emi_dstack_push      (Dstack *dstack, void *data);
void    emi_dstack_pushArray (Dstack *dstack, void *data, int array_length);


/*--------------- UTILITY FUNCTIONS ---------------*/
int     emi_dstack_size      (Dstack *dstack);
int     emi_dstack_dataSize  (Dstack *dstack);
bool    emi_dstack_isEmpty   (Dstack *dstack);

/*--------------- MEMORY MANAGEMENT FUNCTIONS ---------------*/
void    emi_dstack_clear     (Dstack *dstack);
void    emi_dstack_free      (Dstack *dstack);






/*--------------- FUNCTIONS TO ADD ---------------*/
/*
    this one could be in a library that binds them all together
void  emi_dstack_extendByList         (Dstack *dstack, List *data);
extend by Dlist, Llist
*/

#endif
