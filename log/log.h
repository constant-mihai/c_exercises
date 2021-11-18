#pragma once
/* 
 * TODO it's not multithreading safe
 */

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define USE_COLORS 0
#define MAX_BUFFER_SIZE 1024

#if USE_COLORS
#define LOG_RED     "\x1b[31m"
#define LOG_GREEN   "\x1b[32m"
#define LOG_YELLOW  "\x1b[33m"
#define LOG_BLUE    "\x1b[34m"
#define LOG_MAGENTA "\x1b[35m"
#define LOG_CYAN    "\x1b[36m"
#define LOG_RESET   "\x1b[0m"

#else
#define LOG_RED     " !!! CRIT:"
#define LOG_GREEN   "INFO:"
#define LOG_YELLOW  "  !  ERRO:"
#define LOG_BLUE    "WARN:"
#define LOG_MAGENTA "DEBG:"
#define LOG_CYAN    "MEMO:"
#define LOG_RESET   "RESET:"

#endif
// Log level format
#define LOG_LEVEL_FORMAT "<%s>"
#define LOG_LEVEL_VALUE(level) log_get_level_string(level)

// timestamp format
#define LOG_TIME_FORMAT "%d-%02d-%02dT%02d:%02d:%02d.%ldZ"
#define LOG_TIME_VALUE(_tm, _ts) _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec, _ts.tv_nsec / 4

// Message format
#define LOG_HOST_FORMAT "%s"

// App name format
#define LOG_APP_NAME_FORMAT "%s"

// Location format
#define LOG_LOC_FORMAT "[%s():%s:%u]"
#define LOG_LOC_VALUE(_func, _file, _line) _func, _file, _line

// Message format
#define LOG_THREAD_FORMAT "%s:"

// Explicitly add a new line to the end of the logs.
#define LOG_NEWLINE "\n"

#define LOG_HEADER_FORMAT LOG_TIME_FORMAT " " LOG_HOST_FORMAT " " LOG_APP_NAME_FORMAT " " \
                          LOG_LEVEL_FORMAT " " LOG_LOC_FORMAT " " LOG_THREAD_FORMAT " "
#define LOG_HEADER_VAL LOG_TIME_VALUE(tm, ts), hostname_s, appname_s, \
                       LOG_LEVEL_VALUE(level), LOG_LOC_VALUE(func, file, line), threadname_s

typedef enum {
    L_DEFAULT = 0,
    L_CRIT    = 1,
    L_ERR     = 2,
    L_INFO    = 3,
    L_WARN    = 4,
    L_DBG     = 5,
    L_MEM     = 6,
}LoggingLevels;

typedef struct {
    int log_to_console;
    uint8_t level;
    const char *filename;
}log_config_t;

void log_sprintf(int module_idx,
                 int level,
                 const char* func,
                 const char* file,
                 int line,
                 const char *fmt, ...);

// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#define PRINTF(module_idx, level, msg, ...) \
    do {\
        log_sprintf((module_idx), (level), __func__, __FILE__, __LINE__, msg , ##__VA_ARGS__);\
        _log_flush(module_idx);\
    }while(0) 

#define LOG(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_CRIT(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_CRIT, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_ERR(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_ERR, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_WARN(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_WARN, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_INFO(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_DBG(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_DBG, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_MEM(msg, ...) \
    do{\
        PRINTF(module_idx_g, L_MEM, msg, ##__VA_ARGS__);\
    } while(0)

int log_open_fd(const char* filename);

void log_init(const char* appname);

void log_set_thread_name(const char* threadname);

int log_add_module(const char* name, const char* appname, log_config_t config);

void _log_flush();

void log_set_Level(int idx, uint8_t lvl);

uint8_t log_get_level();

const char* log_get_level_string(uint8_t level);

void log_close();
