#pragma once

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>

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

char log_internal_buffer_g[MAX_BUFFER_SIZE];
int log_file_fd_g;
int log_seek_g;
uint8_t log_level_g;

typedef struct {
    int log_to_console;
    const char *filename;
}log_config_t;

log_config_t log_config_g;

#define SPRINTF(buffer, msg, ...)\
    do{\
        sprintf((buffer), msg LOG_HEADER_VAL, ##__VA_ARGS__);\
    }while(0)

#define PRINTF(msg, ...) \
    do {\
        SPRINTF(log_internal_buffer_g, msg, ##__VA_ARGS__);\
        _log_flush();\
    }while(0) 

#define LOG(msg, ...) \
    do{\
        PRINTF(LOG_GREEN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_CRIT(msg, ...) \
    do{\
        PRINTF(LOG_RED LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_ERR(msg, ...) \
    do{\
        PRINTF(LOG_YELLOW LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_WARN(msg, ...) \
    do{\
        PRINTF(LOG_BLUE LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_INFO(msg, ...) \
    do{\
        PRINTF(LOG_GREEN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_DBG(msg, ...) \
    do{\
        PRINTF(LOG_MAGENTA LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_MEM(msg, ...) \
    do{\
        PRINTF(LOG_CYAN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
    } while(0)

#define LOG_AT(l_type, msg, ...) \
    do{\
        if (log_get_level()>=l_type) {\
            switch(l_type) {\
                case L_CRIT:\
                            PRINTF(LOG_RED LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_ERR:\
                           PRINTF(LOG_YELLOW LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_WARN:\
                            PRINTF(LOG_BLUE LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_INFO:\
                            PRINTF(LOG_GREEN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_DBG:\
                           PRINTF(LOG_MAGENTA LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                case L_MEM:\
                           PRINTF(LOG_CYAN LOG_HEADER_FORMAT msg "\n", ##__VA_ARGS__);\
                break;\
                default:\
                        PRINTF(LOG_HEADER_FORMAT msg, ##__VA_ARGS__);\
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

void log_open_fd(const char* filename) {
    int fd = open (filename, O_WRONLY);
    if (fd == -1) {
        perror("error opening log file");
    }
    log_file_fd_g = fd;
    log_seek_g = 0;
}

void log_init(log_config_t config) {
    //bzero(&log_config_g, sizeof(log_config_t));
    memcpy(&log_config_g, &config, sizeof(log_config_t));
    if (log_config_g.filename != NULL) {
        log_open_fd(log_config_g.filename);
    }
}

void _log_flush() {
    if (log_config_g.log_to_console == 1) {
        printf("%s", log_internal_buffer_g);
    }
    if (log_file_fd_g > 0) {
        int seek = lseek(log_file_fd_g, 0, SEEK_END);
        if (seek == -1) {
            perror("error appending to log file.");
        }
        int nr = write(log_file_fd_g,
                       log_internal_buffer_g,
                       strlen(log_internal_buffer_g));
        if (nr == -1) {
            perror("write error");
        } 
    }
} 

void log_set_Level(uint8_t lvl) { log_level_g = lvl; };

uint8_t log_get_level() { return log_level_g; }

void log_close(){
    if (log_file_fd_g != -1) {
        // TODO what happens if buffer isn't flushed?
        if (close(log_file_fd_g == -1)) {
            perror("closing the log file");
        }
    }
}