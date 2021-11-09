#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MAX_Q_R 1000
#define MAX_Q_C 1000

#define LOG_ENABLE 1

#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) do { if (LOG_ENABLE == 1) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__); } while (0)

long arrayManipulation(int n, int queries_rows, int queries_columns, int queries[MAX_Q_R][MAX_Q_C]) {
    (void)queries_columns;
    int **mat = calloc(queries_rows+1, sizeof(int*));
    for (int i=0; i<queries_rows+1; i++) {
        mat[i] = calloc(n, sizeof(int));
    }

    int largest = 0;
    for (int i=0; i<queries_rows; i++) {
        int a = queries[i][0];
        int b = queries[i][1];
        int k = queries[i][2];
        //LOG("%d %d\n", a, b);
        for (int j=0 ; j<n; j++) {
            mat[i+1][j] = mat[i][j];
            if (j >= a-1 /* 1-indexed */ &&  j<b) {
                mat[i+1][j] += k;
            }
            
            //LOG("%d ", mat[i+1][j]);
            if (largest < mat[i+1][j]) {
                largest = mat[i+1][j];
            }
        }
        //LOG("\n");
    }
    for (int i=0; i<queries_rows+1; i++) {
        for (int j=0; j<n; j++) {
            printf("%5d ", mat[i][j]);
        }
        printf("\n");
    }
    for (int i=0; i<queries_rows+1; i++) {
        free(mat[i]);
    }
    free(mat);
    return largest;
}

int main() 
{
    int queries_1[MAX_Q_R][MAX_Q_C] = {
        { 1, 5, 3},
        { 4, 8, 7},
        { 6, 9, 1},
    };
    int queries_2[MAX_Q_R][MAX_Q_C] = {
        { 29, 40, 787 },
        { 9, 26, 219  },
        { 21, 31, 214 },
        { 8, 22, 719  },
        { 15, 23, 102 },
        { 11, 24, 83  },
        { 14, 22, 321 },
        { 5, 22, 300  },
        { 11, 30, 832 },
        { 5, 25, 29   },
        { 16, 24, 577 },
        { 3, 10, 905  },
        { 15, 22, 335 },
        { 29, 35, 254 },
        { 9, 20, 20   },
        { 33, 34, 351 },
        { 30, 38, 564 },
        { 11, 31, 969 },
        { 3, 32, 11   },
        { 29, 35, 267 },
        { 4, 24, 531  },
        { 1, 38, 892  },
        { 12, 18, 825 },
        { 25, 32, 99  },
        { 3, 39, 107  },
        { 12, 37, 131 },
        { 3, 26, 640  },
        { 8, 39, 483  },
        { 8, 11, 194  },
        { 12, 37, 502 },
    };
    LOG("%ld\n", arrayManipulation(10, 3, 3, queries_1));
    LOG("%ld\n", arrayManipulation(40, 30, 3, queries_2));
}
