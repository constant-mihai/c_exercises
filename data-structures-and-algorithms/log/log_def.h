#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    LOG_LEVEL_DEFAULT = 0,
    LOG_LEVEL_FATAL   = 1,
    LOG_LEVEL_ERROR   = 2,
    LOG_LEVEL_INFO    = 3,
    LOG_LEVEL_WARN    = 4,
    LOG_LEVEL_DEBUG   = 5,
    LOG_LEVEL_TRACE   = 6,
} LoggingLevels;

typedef struct {
    int log_to_console;
    uint8_t level;
    char *filename;
} log_config_t;

void log_create(const char* appname, const char* threadname);
int log_find_module(const char* name);
int log_add_module(const char* name, log_config_t config);
void log_sprintf(size_t module_idx,
                 int level,
                 const char* func,
                 const char* file,
                 int line,
                 const char *fmt, ...);
void log_set_Level(int idx, uint8_t lvl);
uint8_t log_get_level();
const char* log_get_level_string(uint8_t level);
void log_destroy();

// machine readable
void mr_snprintf(size_t module_idx, const char* msg, ...);
void mr_log_preamble(size_t module_idx,
                     int level,
                     const char* func,
                     const char* file,
                     int line,
                     const char *msg);
void mr_log_error(const char* error);
void mr_log_buffer(const char* label, int len, const char* buf);
void mr_log_string(const char* label, const char* buf);
void mr_log_uint64(const char* label, uint64_t val);
