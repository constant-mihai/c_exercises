#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'getTotalX' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY a
 *  2. INTEGER_ARRAY b
 */
 
#define LOG_ENABLED 0
#define LOG(msg, ...) do { if (LOG_ENABLED == 1) printf(msg, ##__VA_ARGS__); } while (0)

int gcd(int a, int b) {
   return b == 0 ? a : gcd(b, a%b);
}

int get_greatest_common_divisor(int a_count, int *a) {
    int ret = a[0];
    if (a_count == 1) return ret;
    for (int i=1; i<a_count; i++) {
        LOG("gcd(%d, %d) = \n", ret, a[i]);
        ret = gcd(ret, a[i]); 
        LOG("            = %d\n", ret);
    }
    
    return ret;
}

// https://en.wikipedia.org/wiki/Least_common_multiple
/*
In other words, the least element is increased by the corresponding x whereas the rest of the elements pass from X(m) to X(m+1) unchanged.

The algorithm stops when all elements in sequence X(m) are equal. Their common value L is exactly lcm(X).

For example, if X = X(1) = (3, 4, 6), the steps in the algorithm produce:

X(2) = (6, 4, 6)
X(3) = (6, 8, 6)
X(4) = (6, 8, 12) - by choosing the second 6
X(5) = (9, 8, 12)
X(6) = (9, 12, 12)
X(7) = (12, 12, 12) so lcm = 12.
*/

int check_lcm_condition(int len, long *a) {
    int all_equal = 1;
    for (int i=0; i<len-1; i++) {
        if (a[i] > 100 || a[i+1] > 100) { all_equal = -1; break; } 
        if (a[i] != a[i+1]) { all_equal = 0; }
    }
    
    return all_equal;
}

void increase_smallest_idx(int len, int *a, long *l) {
    int smallest_idx = 0;
    for (int i=0; i<len; i++) {
        if (l[smallest_idx] >= l[i]) {
            smallest_idx = i;
        }
    }
    
    l[smallest_idx]+=a[smallest_idx];
    for (int i=0; i<len; i++) {
        LOG("lcm[%d] = %ld \n", i, l[i]);
    }
    LOG("\n");
}

long get_lcm(int len, int *a) {
    int condition = 0;
    long *lcm = calloc(len, sizeof(long));
    for (int i=0; i<len; i++) {
        lcm[i] = a[i];
    }
    
    condition = check_lcm_condition(len, lcm);
    while(!condition) {
        increase_smallest_idx(len, a, lcm);
        condition = check_lcm_condition(len, lcm);
        if (condition == -1) {
            return -1;
        }
    }
    
    return lcm[0];
}

int getTotalX(int a_count, int* a, int b_count, int* b) {
    // find the smallest common multiple for array a.
    long lcm = get_lcm(a_count, a);

    if (lcm < 0) {
        return 0;
    }

    long mlcm = 1;
    
    int count = 0;
    LOG("lcm array a: %ld\n", lcm);
    long b_gcd = get_greatest_common_divisor(b_count, b);
    LOG("gcd array b: %ld\n", b_gcd);
    
    if (lcm < 0 || b_gcd < lcm) return 0;
    
    for (int i=2; mlcm<=b_gcd; i++) {
        LOG("mlcm a: %ld\n", mlcm);
        if (b_gcd % mlcm == 0) count++;
        mlcm = lcm * i;
    }
    
    return count;
}

int main()
{
    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int m = parse_int(*(first_multiple_input + 1));

    char** arr_temp = split_string(rtrim(readline()));

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(*(arr_temp + i));

        *(arr + i) = arr_item;
    }

    char** brr_temp = split_string(rtrim(readline()));

    int* brr = malloc(m * sizeof(int));

    for (int i = 0; i < m; i++) {
        int brr_item = parse_int(*(brr_temp + i));

        *(brr + i) = brr_item;
    }

    int total = getTotalX(n, arr, m, brr);

    printf( "%d\n", total);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
