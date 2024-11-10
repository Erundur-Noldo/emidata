#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "dlist.h"
#include "common.h"


/*--------------- INTERNAL FUNCTIONS ---------------*/
int _emi_dlist_grow(Dlist *dlist, int goal_size) {
    /* grows the dlist either by the growth exponential,
    or to be large enough to fit in the new size. If -1 is
    provided for the new size, the dlist is forced to
    grow by the growth exponential 
    0 is returned in case of success, 1 in case of failure */
    if (goal_size <= dlist->max_size && goal_size != -1) {
        return 0;
    }

    int new_max_size = dlist->max_size * dlist->growth_exponential;
    if(new_max_size < goal_size) new_max_size = goal_size;

    char *new_location = (char*) realloc (dlist->data, new_max_size * dlist->data_size);
    if(new_location == NULL) {
        printf("reallocation failed. tried to give %d bytes, also, goal was %d\n", new_max_size * dlist->data_size, goal_size * dlist->data_size);
        return 1;
    }
    dlist->data = new_location;
    dlist->max_size = new_max_size;

    return 0;
}

int _emi_dlist_shrink(Dlist *dlist, int goal_size) {
    /* I never actually use this in any of the functions,
    but it's nice to have, I guess?? */
    if(goal_size > dlist->max_size) {
        return 0;
    }
    if(goal_size == -1) {
        goal_size = dlist->max_size / dlist->growth_exponential;
    }
    if(goal_size < emi_dlist_size(dlist)) {
        goal_size = emi_dlist_size(dlist);
    }

    char *new_location = (char*) realloc (dlist->data, goal_size * dlist->data_size);
    if(new_location == NULL) {
        printf("reallocation failed. tried to give %d bytes, also, goal was %d\n", goal_size * dlist->data_size, goal_size * dlist->data_size);
        return 1;
    }
    dlist->data = new_location;
    dlist->max_size = goal_size;

    return 0;
}







/*--------------- CREATION FUNCTIONS ---------------*/
Dlist *emi_dlist_create(int data_size, int data_type) {
    return emi_dlist_createWithParas(data_size, data_type, DEFAULT_INITIAL_SIZE, DEFAULT_GROWTH_EXPONENTIAL);
}


Dlist *emi_dlist_createWithParas(int data_size, int data_type, int initial_size, float growth_exponential) {
    Dlist *new_dlist = (Dlist*) malloc (sizeof(Dlist));
    char *data = (char*) malloc (initial_size * data_size);

    new_dlist->data_size          = data_size;
    new_dlist->data_type          = data_type;
    new_dlist->size               = 0;
    new_dlist->max_size           = initial_size;
    new_dlist->growth_exponential = growth_exponential;
    new_dlist->data               = data;

    return new_dlist;
}


Dlist *emi_dlist_createFromArray (void *data, int array_length, int data_size, int data_type) {
    Dlist *new_dlist = emi_dlist_createWithParas(data_size, data_type, data_size*DEFAULT_GROWTH_EXPONENTIAL, DEFAULT_GROWTH_EXPONENTIAL);
    emi_dlist_extendByArray(new_dlist, data, array_length);
    return new_dlist;
}


Dlist *emi_dlist_createCopy(Dlist *original) {
    Dlist *new_dlist = emi_dlist_createWithParas(original->data_size, original->data_type, original->max_size, original->growth_exponential);
    new_dlist->size = original->size;
    memcpy(new_dlist->data, original->data, original->size * original->data_size);
    return new_dlist;
}

Dlist *emi_dlist_createSublist(Dlist *original, int start_index, int end_index) {
    _common_fixIndexInclusive(original->size, &start_index);
    _common_fixIndexInclusive(original->size, &end_index);

    int new_dlist_size = end_index - start_index;

    if(new_dlist_size < 0)
        return NULL;

    Dlist *new_dlist = emi_dlist_createWithParas(original->data_size, original->data_type, new_dlist_size, original->growth_exponential);
    new_dlist->data = (char*) malloc (original->max_size * original->data_size);
    new_dlist->size = new_dlist_size;

    char* data_start = emi_dlist_readRaw(original, start_index);

    memcpy(new_dlist->data, data_start, new_dlist_size * original->data_size);
    return new_dlist;
}

Dlist *emi_dlist_createSplit(Dlist *original, int index) {
    _common_fixIndex(original->size+1, &index);
    Dlist* output = emi_dlist_createSublist(original, index, -1);
    original->size = index;
    return output;
}


/*--------------- READING FUNCTIONS ---------------*/
void *emi_dlist_read(Dlist *dlist, int index) {
    if(emi_dlist_size(dlist) == 0) {
        printf("can't read from empty dlist\n");
        return NULL;
    }
    char *output = (char *) malloc (dlist->data_size);
    /* index handling is done inside readRaw */
    memcpy(output, emi_dlist_readRaw(dlist, index), dlist->data_size);
    return output;
}

void *emi_dlist_readRaw(Dlist *dlist, int index) {
    if(emi_dlist_size(dlist) == 0) {
        printf("can't read from empty dlist\n");
        return NULL;
    }
    _common_fixIndex(emi_dlist_size(dlist), &index);
    return dlist->data + index * dlist->data_size;
}




/*--------------- MODIFICATION FUNCTIONS ---------------*/
void emi_dlist_append(Dlist *dlist, void *data) {
    if(_emi_dlist_grow(dlist, emi_dlist_size(dlist) + 1) == 1) {
        printf("can't append :(\n");
        return;
    }
    void *new_item;
    if(emi_dlist_size(dlist) == 0) {
        new_item = dlist->data;
    } else {
        new_item = emi_dlist_readRaw(dlist, -1) + dlist->data_size;
    }
    memcpy(new_item, data, dlist->data_size);
    (dlist->size)++;
    return;
}

void emi_dlist_prepend(Dlist *dlist, void *data) {
    emi_dlist_insert(dlist, data, 0);
}

void emi_dlist_insert(Dlist *dlist, void *data, int index) {
    if(_emi_dlist_grow(dlist, emi_dlist_size(dlist) + 1) == 1) {
        printf("can't append :(\n");
        return;
    }
    _common_fixIndexInclusive((emi_dlist_size(dlist)) + 1, &index);

    char *next_item;
    if(index == -1) { /* if it is an empty list, just put it as the first element */
        next_item = dlist->data;
    } else {
        next_item = emi_dlist_readRaw(dlist, -1) + dlist->data_size;
    }

    for(int i=emi_dlist_size(dlist); i>index; i--) {
        memcpy(next_item, next_item - dlist->data_size, dlist->data_size);
        next_item -= dlist->data_size;
    }

    memcpy(next_item, data, dlist->data_size);
    (dlist->size)++;
    return;
}



void emi_dlist_remove(Dlist *dlist, int index) {
    if(emi_dlist_size(dlist) == 0) {
        printf("can't remove from empty dlist\n");
        return;
    }
    _common_fixIndex(emi_dlist_size(dlist), &index);

    char* current_item = emi_dlist_readRaw(dlist, index);    
    for(int i=index; i<(emi_dlist_size(dlist))-1; i++) {
        memcpy(current_item, current_item+dlist->data_size, dlist->data_size);
        current_item += dlist->data_size;
    }
    dlist->size -= 1;
    return;
}

void *emi_dlist_pop(Dlist *dlist) {
    if(emi_dlist_size(dlist) == 0) {
        printf("can't pop from empty dlist\n");
        return NULL;
    }
    char *output = (char *) malloc (dlist->data_size);
    if(output == NULL) {
        printf("malloc failed in emi_dlist_pop :(\n");
        return NULL;
    }
    memcpy(output, emi_dlist_readRaw(dlist, emi_dlist_size(dlist) - 1), dlist->data_size);
    dlist->size -= 1;
    return output;
}


void emi_dlist_set(Dlist *dlist, void *data, int index) {
    if(emi_dlist_size(dlist) == 0) {
        printf("can't set to empty dlist\n");
        return;
    }
    _common_fixIndex(emi_dlist_size(dlist), &index);
    memcpy(emi_dlist_readRaw(dlist, index), data, dlist->data_size);
    return;
}



void emi_dlist_swap(Dlist *dlist, int index_one, int index_two) {
    if(emi_dlist_size(dlist) == 0) {
        printf("can't swap in empty dlist\n");
        return;
    }
    _common_fixIndex(emi_dlist_size(dlist), &index_one);
    _common_fixIndex(emi_dlist_size(dlist), &index_two);
    char* ptr_one = emi_dlist_readRaw(dlist, index_one);
    char* ptr_two = emi_dlist_readRaw(dlist, index_two);
    char buffer[dlist->data_size];

    memcpy(buffer, ptr_one, dlist->data_size);
    memcpy(ptr_one, ptr_two, dlist->data_size);
    memcpy(ptr_two, buffer, dlist->data_size);
    return;
}


void emi_dlist_extendByArray(Dlist *dlist, void *data, int array_length) {
    if(_emi_dlist_grow(dlist, emi_dlist_size(dlist) + array_length) == 1) {
        printf("can't extend :(\n");
        return;
    }
    memcpy(&dlist->data[emi_dlist_size(dlist) * dlist->data_size], data, array_length * dlist->data_size);
    dlist->size += array_length;
    return;
}

void emi_dlist_extendByDlist (Dlist *dlist, Dlist *data) {
    if(_emi_dlist_grow(dlist, emi_dlist_size(dlist) + data->size) == 1) {
        printf("can't extend :(\n");
        return;
    }

    char *source = data->data;
    char *destination = emi_dlist_readRaw(dlist, -1) + dlist->data_size;
    memcpy(destination, source, data->size * data->data_size);
    dlist->size += data->size;
    return;
}






// /*--------------- ORDER CHANGING FUNCTIONS ---------------*/
void emi_dlist_randomizeOrder(Dlist *dlist) {
    for(int i=0; i<emi_dlist_size(dlist) - 1; i++) {
        int j = i + rand() % (emi_dlist_size(dlist) - i);
        emi_dlist_swap(dlist, i, j);
    }
    return;
}



void emi_dlist_bubbleSort(Dlist *dlist) {
    for(int i=emi_dlist_size(dlist) - 1; i>0; i--) {
        char *current_item = dlist->data;
        for(int j=0; j<i; j++) {
            if(_common_defaultOrder(dlist->data_type, current_item, current_item + dlist->data_size) == -1) {
                emi_dlist_swap(dlist, j, j+1);
            }
            current_item += dlist->data_size;
        }
    }

    return;
} /* very shitty */





void emi_dlist_bubbleSortByOrder(Dlist *dlist, int(*order)(void*, void*)) {
    for(int i=emi_dlist_size(dlist) - 1; i>0; i--) {
        char *current_item = dlist->data;
        for(int j=0; j<i; j++) {
            if(order(current_item, current_item + dlist->data_size) == -1) {
                emi_dlist_swap(dlist, j, j+1);
            }
            current_item += dlist->data_size;
        }
    }

    return;

}






void emi_dlist_reverse(Dlist *dlist) {
    for(int i=0; i<emi_dlist_size(dlist) / 2; i++) {
        emi_dlist_swap(dlist, i, emi_dlist_size(dlist) - i - 1);
    }
    return;
}


// /*--------------- THINNENING CHANGING FUNCTIONS ---------------*/
void emi_dlist_filter(Dlist *dlist, bool(*condition)(void*)) {
    bool keep[emi_dlist_size(dlist)];

    char *current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        keep[i] = condition(current_item);
        current_item += dlist->data_size;
    }

    char *current_read  = dlist->data;
    char *current_write = dlist->data;
    int new_size = 0;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        if(keep[i]) {
            if(current_read != current_write)
                memcpy(current_write, current_read, dlist->data_size);
            current_write += dlist->data_size;
            new_size++;
        }
        current_read += dlist->data_size;
    }

    dlist->size = new_size;
    return;
}




void emi_dlist_removeDuplicates(Dlist *dlist) {

    bool delete[emi_dlist_size(dlist)];
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        delete[i] = false;
    }

    char *current_item = dlist->data + dlist->data_size;
    for(int i=1; i<emi_dlist_size(dlist); i++) {
        char *compare_item = dlist->data;
        for(int j=0; j<i; j++) {
            if(memcmp(current_item, compare_item, dlist->data_size) == 0) {
                delete[i] = true;
                break;
            }
            compare_item += dlist->data_size;
        }
        current_item += dlist->data_size;
    }

    char *current_read  = dlist->data;
    char *current_write = dlist->data;
    int new_size = 0;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        if(!delete[i]) {
            if(current_read != current_write)
                memcpy(current_write, current_read, dlist->data_size);
            current_write += dlist->data_size;
            new_size++;
        }
        current_read += dlist->data_size;
    }

    dlist->size = new_size;
    return;
}



// /*--------------- OUTPUT FUNCTIONS ---------------*/
void emi_dlist_print(Dlist *dlist) {
    if(dlist->data == NULL) {
        printf("the data is NULL, can't print\n");
        return;
    }
    if(dlist->data_type == DATA_TYPE_DEF) {
        printf("can't print default data type\n");
        return;
    }

    char *current_item = dlist->data;
    printf("{");
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        _common_printData(current_item, dlist->data_size, dlist->data_type);
        if(i < emi_dlist_size(dlist) - 1) {
            printf(", ");
        }
        current_item += dlist->data_size;
    }
    printf("}\n");
    return;
}

void emi_dlist_printString(Dlist *dlist) {
    if(dlist->data == NULL) {
        printf("the data is NULL, can't print\n");
        return;
    }
    // if(dlist->data_type != DATA_TYPE_STR) {
    //     printf("can only print chars as strings\n");
    //     return;
    // }
    
    _common_printData(dlist->data, emi_dlist_size(dlist), DATA_TYPE_STR);

    // for(int i=0; i<emi_dlist_size(dlist); i++)
    //     _common_printData(dlist->data, dlist->data_size, dlist->data_type);
    return;
}

void emi_dlist_sprintString(char *buffer, Dlist *dlist) {
  if(dlist->data == NULL) {
    printf("the data is NULL, can't print\n");
    return;
  }
  if(buffer == NULL) {
    printf("the buffer is NULL, can't print\n");
    return;
  }

  sprintf(buffer, "%.*s", emi_dlist_size(dlist), (char*) dlist->data);
  buffer[emi_dlist_size(dlist)] = '\0';
  return;
}


// /*--------------- SEARCHING FUNCTIONS ---------------*/
int emi_dlist_find(Dlist *dlist, void *data) {
    char* current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        if(memcmp(current_item, data, dlist->data_size) == 0)
            return i;
        current_item += dlist->data_size;
    }

    return -1; /*if the item doesn't exist*/
}

int emi_dlist_findByCondition(Dlist *dlist, bool(*condition)(void*)) {
    char* current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        if(condition(current_item))
            return i;
        current_item += dlist->data_size;
    }

    return -1; /*if the item doesn't exist*/
}


Dlist *emi_dlist_findAll(Dlist *dlist, void *data) {
    Dlist *output = emi_dlist_create(sizeof(int), DATA_TYPE_INT);

    char* current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        if(memcmp(current_item, data, dlist->data_size) == 0)
            emi_dlist_append(output, &i);
        current_item += dlist->data_size;
    }

    return output; /*if the item doesn't exist*/
}



Dlist *emi_dlist_findAllByCondition (Dlist *dlist, bool(*condition)(void*)) {
    Dlist *output = emi_dlist_create(sizeof(int), DATA_TYPE_INT);

    char* current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        if(condition(current_item))
            emi_dlist_append(output, &i);
        current_item += dlist->data_size;
    }

    return output; /*if the item doesn't exist*/
}




// /*--------------- SET THEORY FUNCTIONS ---------------*/
Dlist *emi_dlist_uniqueElements(Dlist *dlist) {
    Dlist *new_dlist = emi_dlist_createCopy(dlist);
    emi_dlist_removeDuplicates(new_dlist);
    return new_dlist;
}

Dlist *emi_dlist_intersection(Dlist *emi_dlist_one, Dlist *emi_dlist_two) {
    if(emi_dlist_one->data_size != emi_dlist_two->data_size)
        printf("!!!BIG WARNING!!! you're tryna intersect a list with a list that has another datasize\n");
    if(emi_dlist_one->data_type != emi_dlist_two->data_type)
        printf("!small warning! you're tryna intersect a list with a list that has another datatype\n");

    /* we're looping through list_two a lot, but only once 
    through list_one, so if list_two is larger, we should swap
    them */
    if (emi_dlist_one->size < emi_dlist_two->size) {
        Dlist *buffer = emi_dlist_one;
        emi_dlist_one = emi_dlist_two;
        emi_dlist_two = buffer;
    }
    
    /* we take the size of the second array, since the maximal size is that 
    of the smaller of the two arrays, which is the second one */
    Dlist *output = emi_dlist_createWithParas(emi_dlist_one->data_size, emi_dlist_one->data_type, emi_dlist_two->size, emi_dlist_two->growth_exponential);


    char *current_item = emi_dlist_one->data;
    for(int i=0; i<emi_dlist_one->size; i++) {
        if(emi_dlist_find(emi_dlist_two, current_item) != -1)
            emi_dlist_append(output, current_item);
        current_item += emi_dlist_one->data_size;
    }

    emi_dlist_removeDuplicates(output);
    return output;
}


Dlist *emi_dlist_union(Dlist *emi_dlist_one, Dlist *emi_dlist_two) {
    Dlist *output = emi_dlist_createCopy(emi_dlist_one);
    emi_dlist_extendByDlist(output, emi_dlist_two);
    emi_dlist_removeDuplicates(output);
    return output;
}






// /*--------------- COMPUTATIONAL FUNCTIONS ---------------*/
void emi_dlist_map(Dlist *dlist, void(*map)(void*, void*)) {
    /* the map function should have the first argument
    be of the same type as the list contains, and the
    second one as well, and it's where the output is
    written */
    char buffer[dlist->data_size];
    char *current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        /* we use a buffer in case the result data is used
        multiple times. Otherwise, the map could try to
        reuse the data, even if we have already changed it
        (in case we would do map(current_item, cuttent_item);,
        that is) */
        map(current_item, buffer);
        memcpy(current_item, buffer, dlist->data_size);
        current_item += dlist->data_size;
    }
    return;
}

void emi_dlist_reduce(Dlist *dlist, void(*map)(void*, void*), void *output) {
    /* the map function should have the first argument
    be of the same type as the list contains, and the
    second one of the desired output type, and it's 
    where the output is written
    I do in fact choose to work with an output in this
    way right here, instead of using malloc, because
    otherwise the size of the output *and* the starting
    state of the output would have to be passed as
    arguments. Like this, the starting state is simply
    whatever is at output at the calling of the function,
    and the size is implied in how map uses it 
    nice and long comment :3 */
    char *current_item = dlist->data;
    for(int i=0; i<emi_dlist_size(dlist); i++) {
        map(current_item, output);
        current_item += dlist->data_size;
    }

    return;
}







// /*--------------- METADATA FUNCTIONS ---------------*/
int emi_dlist_size(Dlist *dlist) {
    return dlist->size; /* x3 */
}
int emi_dlist_dataSize(Dlist *dlist) {
    return dlist->data_size;
}
bool emi_dlist_isEmpty(Dlist *dlist) {
    return dlist->size == 0;
}



// /*--------------- CLEANING FUNCTIONS ---------------*/
void emi_dlist_clear(Dlist *dlist) {
    dlist->size = 0;
    return;
    /* heh~ */
}

void emi_dlist_free(Dlist *dlist) {
    free(dlist->data);
    free(dlist);
    return;
}


