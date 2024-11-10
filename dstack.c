#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "dstack.h"
#include "common.h"


/*--------------- INTERNAL FUNCTIONS ---------------*/
int _emi_dstack_grow(Dstack *dstack, int goal_size) {
    /* grows the dstack either by the growth exponential,
    or to be large enough to fit in the new size. If -1 is
    provided for the new size, the dstack is forced to
    grow by the growth exponential 
    0 is returned in case of success, 1 in case of failure */
    if (goal_size <= dstack->max_size && goal_size != -1) {
        return 0;
    }

    int new_max_size = dstack->max_size * dstack->growth_exponential;
    if(new_max_size < goal_size) new_max_size = goal_size;

    char *new_location = (char*) realloc (dstack->data, new_max_size * dstack->data_size);
    if(new_location == NULL) {
        printf("reallocation failed. tried to give %d bytes, also, goal was %d\n", new_max_size * dstack->data_size, goal_size * dstack->data_size);
        return 1;
    }
    dstack->data = new_location;
    dstack->max_size = new_max_size;

    return 0;
}

int _emi_dstack_shrink(Dstack *dstack, int goal_size) {
    /* I never actually use this in any of the functions,
    but it's nice to have, I guess?? */
    if(goal_size > dstack->max_size) {
        return 0;
    }
    if(goal_size == -1) {
        goal_size = dstack->max_size / dstack->growth_exponential;
    }
    if(goal_size < emi_dstack_size(dstack)) {
        goal_size = emi_dstack_size(dstack);
    }

    char *new_location = (char*) realloc (dstack->data, goal_size * dstack->data_size);
    if(new_location == NULL) {
        printf("reallocation failed. tried to give %d bytes, also, goal was %d\n", goal_size * dstack->data_size, goal_size * dstack->data_size);
        return 1;
    }
    dstack->data = new_location;
    dstack->max_size = goal_size;

    return 0;
}






/*--------------- CREATION FUNCTIONS ---------------*/
Dstack *emi_dstack_create(int data_size, int data_type) {
    return emi_dstack_createWithParas(data_size, data_type, DEFAULT_INITIAL_SIZE, DEFAULT_GROWTH_EXPONENTIAL);
}


Dstack *emi_dstack_createWithParas(int data_size, int data_type, int initial_size, float growth_exponential) {
    Dstack *new_dstack = (Dstack*) malloc (sizeof(Dstack));
    char *data = (char*) malloc (initial_size * data_size);

    new_dstack->data_size          = data_size;
    new_dstack->data_type          = data_type;
    new_dstack->size               = 0;
    new_dstack->max_size           = initial_size;
    new_dstack->growth_exponential = growth_exponential;
    new_dstack->data               = data;

    return new_dstack;
}


Dstack *emi_dstack_createFromArray (void *data, int array_length, int data_size, int data_type) {
    Dstack *new_dstack = emi_dstack_createWithParas(data_size, data_type, data_size*DEFAULT_GROWTH_EXPONENTIAL, DEFAULT_GROWTH_EXPONENTIAL);
    emi_dstack_pushArray(new_dstack, data, array_length);
    return new_dstack;
}


Dstack *emi_dstack_createCopy(Dstack *original) {
    Dstack *new_dstack = emi_dstack_createWithParas(original->data_size, original->data_type, original->max_size, original->growth_exponential);
    new_dstack->size = original->size;
    memcpy(new_dstack->data, original->data, original->size * original->data_size);
    return new_dstack;
}


/*--------------- READING FUNCTIONS ---------------*/
void *emi_dstack_peek(Dstack *dstack) {
    if(emi_dstack_size(dstack) == 0) {
        printf("can't read from empty dstack\n");
        return NULL;
    }
    char *top = emi_dstack_top(dstack);
    if(top == NULL) return NULL;

    char *output = (char *) malloc (dstack->data_size);
    memcpy(output, top, dstack->data_size);
    return output;
}

void *emi_dstack_top(Dstack *dstack) {
    if(dstack->size == 0) {
        printf("tried to read the top of an empty stack\n");
        return NULL;
    }
    return dstack->data + (dstack->size - 1) * dstack->data_size;
}

void *emi_dstack_pop(Dstack *dstack) {
    char *output = (char *) malloc (dstack->data_size);
    if(output == NULL) {
        printf("malloc failed in emi_dstack_pop :(\n");
        return NULL;
    }
    char *top = emi_dstack_top(dstack);
    if(top == NULL) return NULL;
    memcpy(output, top, dstack->data_size);
    (dstack->size)--;
    return output;
}

void *emi_dstack_popRaw(Dstack *dstack) {
    char *top = emi_dstack_top(dstack);
    if(top == NULL) return NULL;
    (dstack->size)--;
    return top;
}

void emi_dstack_popSilent(Dstack *dstack) {
    /* pops and doesn't return anything */
    (dstack->size)--;
    return;
}

/*--------------- MODIFICATION FUNCTIONS ---------------*/
void emi_dstack_push(Dstack *dstack, void *data) {
    if(_emi_dstack_grow(dstack, emi_dstack_size(dstack) + 1) == 1) {
        printf("can't append :(\n");
        return;
    }
    (dstack->size)++; /* we increment first, so it can't be an empty stack anymore */
    char *top = emi_dstack_top(dstack);
    memcpy(top, data, dstack->data_size);
    return;
}

void emi_dstack_pushArray(Dstack *dstack, void *data, int array_length) {
    if(_emi_dstack_grow(dstack, emi_dstack_size(dstack) + array_length) == 1) {
        printf("can't extend :(\n");
        return;
    }
    (dstack->size)++; /* we increase by one already to make the top calculate from where on data has to be added */
    char *top = emi_dstack_top(dstack); /* because of that incremeting, we also know the dstack is not empty, so no need to check that */
    memcpy(top, data, array_length * dstack->data_size);
    dstack->size += array_length - 1; /* minus one because we added one earlier */
    return;
}








// /*--------------- METADATA FUNCTIONS ---------------*/
int emi_dstack_size(Dstack *dstack) {
    return dstack->size; /* x3 */
}
int emi_dstack_dataSize(Dstack *dstack) {
    return dstack->data_size;
}

bool emi_dstack_isEmpty(Dstack *dstack) {
    return dstack->size == 0;
}



// /*--------------- CLEANING FUNCTIONS ---------------*/
void emi_dstack_clear(Dstack *dstack) {
    dstack->size = 0;
    return;
    /* heh~ */
}

void emi_dstack_free(Dstack *dstack) {
    free(dstack->data);
    free(dstack);
    return;
}


