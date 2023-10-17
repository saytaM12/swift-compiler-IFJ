#include "lexical.h"

int read(char* stringFirst) {
    FILE *stream = stdin;
    //#FILE *stream = fopen("r", some_file);
    while (true) {
        char c = getc(stream);

        switch (c) {
            case 'a' ... 'z':
                printf("hi");
                break;

            default:
                break;

        }
    }
}

int main() {
    char a = 'a';
    read(&a);
}
