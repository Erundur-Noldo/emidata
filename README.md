# emidata
a shitty datastructure library for C
only dependent on the C standard library
my cute overengineered pet project which I think I'll use myself when it's done in other projects, unless I discover it's total shit


common.c and common.h contain a few internal functions which are not part of the api, but they're used by the other files. You should always add it to the files that you compile when you use any of the other files.

so far, I have dlist (dynamically allocated arrays), dstack (dynamically allocated stacks)

plans:

improvements:
- making a documentation of how to use the API
- improve the printing function
- add more datatypes
- make better sorting functions
- fix the default order function

additions:
- llist (linked list) (is kinda already done, but I have to rename stuff because I redesigned the naming scheme)
- dllist (doubly linked list)
- lstack (linked stack, the 'top' is the root)
- dqueue (dynamically allocated queue)
- lqueue (linked queue)
- either or both dtree and ltree
- either or both dgraph and lgraph
- variants of graphs (can be directed, weighted, hyper, or any combination of those)
- maybe some sort of 'rope string' I heard of for the first time today??
- maybe a hashmap?? dictionary/map/some other name????
- maybe more?????????????

then when it's fully done, I'll make it into a library file or whatever
