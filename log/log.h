#pragma once
/* TODO While this works well enough, it can be done better.
 *  - Right now the global variables will be shared amongst all modules.
 *  - Compiling them is tricky:
 *    usr/bin/ld: hash.o: relocation R_X86_64_PC32 against symbol `log_file_fd_g' can not be used when making a shared object; recompile with -fPIC
 *
 * This could be made a library and the global variables hidden away.
 * It can then be loaded by each executable/shared library.
 *
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

#define USE_COLORS              0
#define MAX_BUFFER_SIZE 65535

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

#define LOG_HEADER_FORMAT "[%s][%s]%s:%d "
#define LOG_HEADER_VAL ,__TIME__,__FILE__,__func__,__LINE__

typedef struct {
    int log_to_console;
    const char *filename;
}log_config_t;

log_config_t log_config_g;

#define SPRINTF(buffer, msg, ...)\
    do{\
        sprintf((buffer), msg LOG_HEADER_VAL, ##__VA_ARGS__);\
    }while(0)

#define PRINTF(module_idx, msg, ...) \
    do {\
        SPRINTF(log_internal_buffer_g[module_idx], msg, ##__VA_ARGS__);\
        _log_flush();\
    }while(0) 

#define LOG(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_GREEN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_CRIT(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_RED LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_ERR(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_YELLOW LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_WARN(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_BLUE LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_INFO(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_GREEN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_DBG(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_MAGENTA LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_MEM(msg, ...) \
    do{\
        PRINTF(module_idx_g, LOG_CYAN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_AT(l_type, msg, ...) \
    do{\
        if (log_get_level()>=l_type) {\
            switch(l_type) {\
                case L_CRIT:\
                            PRINTF(module_idx_g, LOG_RED LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_ERR:\
                           PRINTF(module_idx_g, LOG_YELLOW LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_WARN:\
                            PRINTF(module_idx_g, LOG_BLUE LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_INFO:\
                            PRINTF(module_idx_g, LOG_GREEN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_DBG:\
                           PRINTF(module_idx_g, LOG_MAGENTA LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_MEM:\
                           PRINTF(module_idx_g, LOG_CYAN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                default:\
                        PRINTF(module_idx_g, LOG_HEADER_FORMAT msg, ##__VA_ARGS__);\
                break;\
            }\
        }\
    }while(0)

typedef enum {
    L_DEFAULT = 0,
    L_CRIT    = 1,
    L_ERR     = 2,
    L_INFO    = 3,
    L_WARN    = 4,
    L_DBG     = 5,
    L_MEM     = 6,
}LoggingLevels;

void log_open_fd(const char* filename);

int log_init(log_config_t config);

void _log_flush();

void log_set_Level(uint8_t lvl);

uint8_t log_get_level();

void log_close();
