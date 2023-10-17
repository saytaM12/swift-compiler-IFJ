#include <stdbool.h>
#include <stdio.h>

enum state{
    identifier = 1,
    number = 2,
    equals = 3,
    operation = 4,
};

int readNext(char* stringFirst);
