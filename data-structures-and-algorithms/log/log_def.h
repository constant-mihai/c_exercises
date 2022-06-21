#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    L_DEFAULT = 0,
    L_CRIT    = 1,
    L_ERR     = 2,
    L_INFO    = 3,
    L_WARN    = 4,
    L_DBG     = 5,
    L_MEM     = 6,
} LoggingLevels;

typedef struct {
    int log_to_console;
    uint8_t level;
    char *filename;
} log_config_t;


void log_sprintf(size_t module_idx,
                 int level,
                 const char* func,
                 const char* file,
                 int line,
                 const char *fmt, ...);

int log_find_module(const char* name);
int log_open_fd(const char* filename);
void log_init(const char* appname); // TODO: deprecate this
void log_create(const char* appname, const char* threadname);
int log_add_module(const char* name, log_config_t config);
void log_set_Level(int idx, uint8_t lvl);
uint8_t log_get_level();
const char* log_get_level_string(uint8_t level);
void log_destroy();

// machine readable
void mr_log_preamble(size_t module_idx,
                     int level,
                     const char* func,
                     const char* file,
                     int line,
                     const char *msg);

int mr_log_error(const char* error);
int mr_log_buffer(const char* label, int len, const char* buf);
int mr_log_string(const char* label, const char* buf);
int mr_log_uint64(const char* label, uint64_t val);
