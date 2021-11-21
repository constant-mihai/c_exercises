#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "select-sort.h"
#include "log/log.h"

void select_sort(int *array, size_t len) {
    int *least = 0;

    for (size_t i=0; i<len; i++) {
        least = &array[i];
        for (size_t j=i; j<len; j++) {
            if (array[j] < *least) {
                least = &array[j];
            }
        }
        if (&array[i] != least) {
            int tmp = array[i];
            array[i] = *least;
            *least = tmp;
        }
    }
}
