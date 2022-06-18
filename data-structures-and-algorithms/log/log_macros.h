#pragma once

#include "log.h"
#include "log_internals.h" 

#define DEFAULT_BUFFER_SIZE 1024
#define DEFAULT_MODULES_NUM 100

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

// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#define PRINTF(module_idx, level, msg, ...) \
    do {\
        log_sprintf((module_idx), (level), __func__, __FILE__, __LINE__, msg , ##__VA_ARGS__);\
        _log_flush(module_idx);\
    }while(0) 

//TODO these overlap with macros in syslog.h 
//
// LOG at default index
//
#define LOG_DEFAULT_MODULE_INDEX 0
#define LOG(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_CRIT(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_CRIT, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_ERR(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_ERR, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_WARN(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_WARN, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_INFO(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_DBG(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_DBG, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_MEM(msg, ...) \
    do{\
        PRINTF(LOG_DEFAULT_MODULE_INDEX, L_MEM, msg, ##__VA_ARGS__);\
    } while(0)

//
// Module based
//
#define LOG_AT(log_mod_idx, msg, ...) \
    do{\
        if (log_mod_idx > 0) PRINTF(log_mod_idx, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_CRIT_AT(log_mod_idx, msg, ...) \
    do{\
        if (log_mod_idx > 0) PRINTF(log_mod_idx, L_CRIT, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_ERR_AT(log_mod_idx, msg, ...) \
    do{\
        if (log_mod_idx > 0) PRINTF(log_mod_idx, L_ERR, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_WARN_AT(log_mod_idx, msg, ...) \
    do{\
        if (log_mod_idx > 0) PRINTF(log_mod_idx, L_WARN, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_INFO_AT(log_mod_idx, msg, ...) \
    do{\
        if (log_mod_idx > 0) PRINTF(log_mod_idx, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_DBG_AT(log_mod_idx, msg, ...) \
    do{\
        if (log_mod_idx > 0) PRINTF(log_mod_idx, L_DBG, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_MEM_AT(log_mod_idx, msg, ...) \
    do{\
        PRINTF(log_mod_idx, L_MEM, msg, ##__VA_ARGS__);\
    } while(0)

#define LOG_CREATE_DEFAULT(log_mod_idx, loglvl) \
    log_create("no-app-name-provided", "main"); \
    do { \
        log_config_t log_config = { \
            .log_to_console = 1, \
            .level = loglvl, \
            .filename = NULL \
        }; \
        log_add_module(log_mod_idx, log_config); \
    } while(0)

#define LOG_ADD_MODULE(log_mod_idx, ltc, loglvl, file_name) \
    do { \
        log_config_t log_config = { \
            .log_to_console = ltc, \
            .level = loglvl, \
            .filename = file_name \
        }; \
        log_add_module(log_mod_idx, log_config); \
    } while(0)

