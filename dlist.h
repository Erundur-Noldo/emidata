/* my personal dynamic array library
by Emika
01-11-2024 to 10-11-2024

  ____
 /    \
| _  _ |
|      |
 \    /
  \  /
   \/
*/



#ifndef ARRAYLIST_H
#define ARRAYLIST_H


#include <stdbool.h>
#include "common.h"


/*--------------- DEFINES ---------------*/
#define DEFAULT_INITIAL_SIZE 16
#define DEFAULT_GROWTH_EXPONENTIAL 2.0


/*--------------- STRUCTS ---------------*/
typedef struct Dlist {
    int data_size;
    int data_type;
    int size;
    int max_size;
    float growth_exponential;
    char *data;
} Dlist;

/*--------------- ENUMS ---------------*/




/*--------------- CREATION FUNCTIONS ---------------*/
Dlist *emi_dlist_create          (int data_size, int data_type);
Dlist *emi_dlist_createWithParas (int data_size, int data_type, int initial_size, float growth_exponential);
Dlist *emi_dlist_createFromArray (void *data, int array_length, int data_size, int data_type);
Dlist *emi_dlist_createCopy      (Dlist *original);
Dlist *emi_dlist_createSublist   (Dlist *original, int start_index, int end_index);
Dlist *emi_dlist_createSplit     (Dlist *original, int index); /* shortens the inputed dlist, and returns the second half */

/*--------------- READING FUNCTIONS ---------------*/
void  *emi_dlist_read    (Dlist *dlist,             int index);
void  *emi_dlist_readRaw (Dlist *dlist,             int index);

/*--------------- MODIFICATION FUNCTIONS ---------------*/
void   emi_dlist_append        (Dlist *dlist, void *data           );
void   emi_dlist_prepend       (Dlist *dlist, void *data           );
void   emi_dlist_insert        (Dlist *dlist, void *data, int index);
void   emi_dlist_remove        (Dlist *dlist,             int index);
void  *emi_dlist_pop           (Dlist *dlist                       );
void   emi_dlist_set           (Dlist *dlist, void *data, int index);
void   emi_dlist_swap          (Dlist *dlist, int index_one, int index_two);
void   emi_dlist_extendByArray (Dlist *dlist, void *data, int array_length);
void   emi_dlist_extendByDlist (Dlist *dlist, Dlist *data);

/*--------------- ORDER CHANGING FUNCTIONS ---------------*/
void   emi_dlist_randomizeOrder       (Dlist *dlist);
void   emi_dlist_bubbleSort           (Dlist *dlist); /* very shitty */
void   emi_dlist_bubbleSortByOrder    (Dlist *dlist, int(*order)(void*, void*));
void   emi_dlist_reverse              (Dlist *dlist);

/*--------------- THINNENING CHANGING FUNCTIONS ---------------*/
void   emi_dlist_filter               (Dlist *dlist, bool(*condition)(void*));
void   emi_dlist_removeDuplicates     (Dlist *dlist);

/*--------------- SEARCHING FUNCTIONS ---------------*/
int    emi_dlist_find                 (Dlist *dlist, void *data);
int    emi_dlist_findByCondition      (Dlist *dlist, bool(*condition)(void*));
Dlist *emi_dlist_findAll              (Dlist *dlist, void *data);
Dlist *emi_dlist_findAllByCondition   (Dlist *dlist, bool(*condition)(void*));

/*--------------- SET OPERATIONS ---------------*/
Dlist *emi_dlist_uniqueElements       (Dlist *dlist);
Dlist *emi_dlist_intersection         (Dlist *dlist_one, Dlist *dlist_two); /* not made yet */
Dlist *emi_dlist_union                (Dlist *dlist_one, Dlist *dlist_two);

/*--------------- COMPUTATIONAL FUNCTIONS ---------------*/
void emi_dlist_map     (Dlist *dlist, void(*map)(void*, void*));
void emi_dlist_reduce  (Dlist *dlist, void(*map)(void*, void*), void* output);

/*--------------- UTILITY FUNCTIONS ---------------*/
int emi_dlist_size     (Dlist *dlist);
int emi_dlist_dataSize (Dlist *dlist);
void emi_dlist_print        (Dlist *dlist);
void emi_dlist_printString  (Dlist *dlist);
void emi_dlist_sprintString (char *buffer, Dlist *dlist);

/*--------------- MEMORY MANAGEMENT FUNCTIONS ---------------*/
void emi_dlist_clear   (Dlist *dlist);
void emi_dlist_free    (Dlist *dlist);
bool emi_dlist_isEmpty (Dlist *dlist);

/*--------------- FUNCTIONS TO ADD ---------------*/
/*
void  emi_dlist_betterSort           (Dlist *dlist);
void  emi_dlist_betterSortByOrder    (Dlist *dlist, int(*order)(void*, void*));
int   emi_dlist_count                (Dlist *dlist, void *data);


    this one could be in a library that binds them all together
void  emi_dlist_extendByList         (Dlist *dlist, List *data);

*/

#endif
