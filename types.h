#ifndef TYPES_H
#define TYPES_H

#define RESET   "\033[0m"
#define RED     "\033[1;91m"
#define GREEN   "\033[1;92m"

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
