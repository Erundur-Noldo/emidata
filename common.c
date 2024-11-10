#include <stdio.h>
#include <stdlib.h>

#include "common.h"


void _common_fixIndex(int size, int *index) {
    /* we check the following case seperately because a lot of times,
    this function will be run on an index which has already been fixes,
    so this saves time since it prevents us from doing
    the *heavy* modulo operation a couple of times */
    if(0 <= *index && *index < size) {
        /* do nothing */
        return;
    }
    if(size == 0) {
        *index = -1;
        return;
    }
    *index = (*index % size + size) % size;
    return;
}


void _common_fixIndexInclusive(int size, int *index) {
    /* this function is for when you are in a situation
    where, if a list has length a, _common_fixIndex(a, &a)
    should keep the index as a, rather than bringing it 
    down to 0 */
    _common_fixIndex(size + 1, index);
    return;
}





int _common_defaultOrder(int data_type, void *a, void *b) {
    switch(data_type) {

    // case DATA_TYPE_CHAR:
    //     return _list_charOrder(a, b);
    //     break;
    // case DATA_TYPE_STR:
    //     for(int i=0; i<list->data_size; i++) {
    //         int this_char = _list_charOrder(a, b);
    //         if(this_char != 0) return this_char;
    //     }
    //     return 0;
    //     break;
    case DATA_TYPE_INT:
        if(*(int*)a == *(int*)b) return 0;
        return (*(int*)a < *(int*)b)*2-1;
        break;
    // case DATA_TYPE_FLOAT:
    //     if(*(float*)a == *(float*)b) return 0;
    //     return (*(float*)a < *(float*)b)*2-1;
    //     break;
    default: /* in other cases (default, pointer, faulty ones) 
    we just give the order where nothing has a relationship 
    with eachother I guess, or everything is equal */
        return 0;
    }
}


int _common_charOrderBlock(char c) {
    if('a' < c && c < 'z') return 4; /* small letters */
    if('A' < c && c < 'Z') return 3; /* uppercase letters */
    if('0' < c && c < '9') return 2; /* numbers */
    if(' ' < c && c < '~') return 1; /* punctuation */
    return 0; /* the characters which can't be printed */
}

int _common_charOrder(void *_a, void *_b) {
    char a = *(char*) _a;
    char b = *(char*) _b;
    if(a == b) 
        return 0;

    /* first, we put both of them into the sort of 'block' they fall into */
    int block_a = _common_charOrderBlock(a);
    int block_b = _common_charOrderBlock(b);

    if(block_a + block_b == 7) {
        /* this can only happen if they are 3 and 4, 
        aka a small and a capital letter */
        if(abs(a - b) == 'a' - 'A') /* they are the same letter */
            return (a < b)*2-1;     /* the capital is counted as higher */

        if(a < b) /* turn both into a small letter */
            a += 'a' - 'A'; 
        else
            b += 'a' - 'A';

        return (a < b)*2-1;
    }

    if(block_a != block_b) {
        return (block_a < block_b)*2-1;
    }

    return (a < b)*2-1;
}








int _common_sizeof(int datatype) {
    switch(datatype) {
        case DATA_TYPE_CHAR:
            return sizeof(char);
            break;
        case DATA_TYPE_INT:
            return sizeof(int);
            break;
        case DATA_TYPE_FLOAT:
            return sizeof(float);
            break;
        case DATA_TYPE_PTR:
            return sizeof(void*);
            break;
        default:
            return -1;
    }
}


void _common_printData(void* data, int datasize, int datatype) {
    /* prints a datapoint in a specific form */
    int array_length = datasize / _common_sizeof(datatype);
    if(array_length < 1) array_length = 1; /* this happens only if _common_sizeof(datatype) returned -1 */

    char *current_data = (char *) data;


    for(int i=0; i<array_length; i++) {
        switch (datatype) {
            case DATA_TYPE_CHAR:
                printf("%c", *(char*) current_data);
                break;

            case DATA_TYPE_STR:
                printf("%.*s", datasize, (char*) current_data);
                break;

            case DATA_TYPE_INT:
                printf("%d", *(int*) current_data);
                break;

            case DATA_TYPE_FLOAT:
                printf("%f", *(float*) current_data);
                break;

            case DATA_TYPE_PTR:
                printf("%p", (void*) current_data);
                break;

            default:
                printf("error: attempted to print 'unprintable' datatype :v\n");
                return;
        }


        current_data += _common_sizeof(datatype);


    }
}