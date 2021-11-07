#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define LOG_ENABLE 1

#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) do { if (LOG_ENABLE == 1) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__); } while (0)

typedef struct {
    char c;
    int count;
} set_el_t;

int insert_set(set_el_t *set, int set_size, char num) {
    // search for the value in the set
    printf("checking set for: %c\n", num);
    int found = 0;
    for(int n=0; n<set_size; n++) {
        if (set[n].c == num) {
            found = 1;
            //if ((num >= 'A' && num <= 'Z') || (num >= 'a' && num <= 'z')) {
            //    goto done;
            //} else
            if (num >= '0' && num <= '9') {
                set[n].count++;
            }
        }
    }

    if (!found) {
        set[set_size].c = num;
        if (num >= '0' && num <= '9') {
            set[set_size].count = 1;
        } else {
            set[set_size].count = 0;
        }
        set_size++;
    }
    return set_size;
}

int main() {

    char s[2000];
    scanf("%s", s);

    int digits[10] = {};

    for (int i=0; i<strlen(s); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            digits[s[i] - '0']++;
        }
    }

    for (int i=0; i<10; i++) {
        printf("%d ", digits[i]);
    }

    //set_el_t set[2000] = {};
    //int set_size = 0;
    //for (int i=0; i<strlen(s); i++) {
    //    set_size = insert_set(set, set_size, s[i]);
    //}
    //
    //for (int i=0; i<set_size; i++) {
    //    printf("%c ", set[i].c);
    //}
    //printf("\n");
    //
    //for (int i=0; i<set_size; i++) {
    //    printf("%d ", set[i].count);
    //}

    return 0;
}
