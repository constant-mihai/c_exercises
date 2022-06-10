#pragma once

#define MALLOC(dst, len, type) \
    do { \
        (dst) = (type*)malloc(len * sizeof(type)); \
        if ((dst) == NULL) { \
            fprintf(stderr, "error: out of memory"); \
            exit(1); \
        } \
    } while(0) 

#define REALLOC(dst, len, type) \
    do { \
        (dst) = (type*)realloc(dst, len * sizeof(type)); \
        if ((dst) == NULL) { \
            fprintf(stderr, "error: out of memory"); \
            exit(1); \
        } \
    } while(0) 

#define CALLOC(dst, num, type) \
    do { \
        (dst) = (type*)calloc(num, sizeof(type)); \
        if ((dst) == NULL) { \
            fprintf(stderr, "error: out of memory"); \
            exit(1); \
        } \
    } while(0) 
