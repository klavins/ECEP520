#include "examples.h"

int * join(const int * a, int length_a, const int * b, int length_b) {
    int * c = (int *) calloc(length_a+length_b, sizeof(int));
    for (int i=0; i<length_a; i++ ) {
        c[i] = a[i];
    }
    for (int i=0; i<length_b; i++ ) {
        c[i+length_a] = b[i];
    }
    return c;
}