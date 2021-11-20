#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

int select_sort() {
    int ta[5] = {5, 0, 4, 3, 2};
    int ta_len = sizeof(ta)/sizeof(ta[0]);
    int *least = 0;

    for (int i=0; i<ta_len; i++) {
        least = &ta[i];
        for (int j=i; j<ta_len; j++) {
            if (ta[j] < *least) {
                least = &ta[j];
            }
        }
        if (&ta[i] != least) {
            int tmp = ta[i];
            ta[i] = *least;
            *least = tmp;
        }
    }

    for (int i=0; i<ta_len; i++) {
        printf("%d\n", ta[i]);
    }

    char a[3] = {'g', 'h', '\0'};
    char b[4] = {'g', 'h', 'j', '\0'};
    printf("a b %s %s\n", a,b);
    printf("a b %lu %lu\n", strlen(a), strlen(b));
    printf("res %d\n", strcmp("gh", "ghj"));
    printf("res %d\n", strncmp("gh", "ghj", 3));
    printf("res %d\n", strcmp(a, b));
    printf("res %d\n", strncmp(a, b, 2));

    return 0;
}
