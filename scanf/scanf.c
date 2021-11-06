#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

int main(int argc, char ** argv) {
    LOG("main");
    //test_insert();
    //test_search();
    //test_remove();

    char input[2000];
    while (scanf("%[^\n]%*c", input) == 1) {
        LOG("input: %s", input);
    }

    return 0;
}
