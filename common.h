#ifndef COMMON_H
#define COMMON_H


typedef enum {
    DATA_TYPE_DEF    = 0, /*  any data  */
    DATA_TYPE_CHAR   = 1, /*  chars are printed as %d  */
    DATA_TYPE_STR    = 2, /*  strings as %s            */
    DATA_TYPE_INT    = 3,
    DATA_TYPE_FLOAT  = 4,
    DATA_TYPE_PTR    = 5, /*  void*  */
} DataType;

// typedef enum {
//     DATA_TYPE_DEF    = 0, /*  any data  */
//     DATA_TYPE_CHAR   = 1,
//     DATA_TYPE_STR    = 2,
//     DATA_TYPE_INT    = 3,
//     DATA_TYPE_FLOAT  = 4,
//     DATA_TYPE_PTR    = 5, /*  void*  */
// } DataType;





/*--------------- ORDER FUNCTIONS ---------------*/
int _common_defaultOrder   (int datatype, void *a, void *b);
int _common_charOrder      (void *_a, void *_b);
int _common_charOrderBlock (char c);



/*--------------- UTILITY FUNCTIONS ---------------*/
void _common_fixIndex          (int size, int *index);
void _common_fixIndexInclusive (int size, int *index);
int  _common_sizeof            (int datatype);
void _common_printData         (void* data, int data_size, int data_type);


#endif

