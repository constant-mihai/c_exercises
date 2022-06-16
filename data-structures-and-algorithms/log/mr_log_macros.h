#pragma once

#include "log.h"

// TODO: need to check if the message size is bigger then the space left in MAX_BUFFER_SIZE.
#define MR_SNPRINTF(msg, ...) \
    do { \
        pos += snprintf(buf+pos, DEFAULT_BUFFER_SIZE-pos-2 /* two chars: } and \n */, msg, ##__VA_ARGS__); \
        if (pos >= DEFAULT_BUFFER_SIZE-2 /* two chars: } and \n */ ) { \
            log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-3] = '}'; \
            log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-2] = '\n'; \
            log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-1] = '0'; \
            va_end(args); \
            return; \
        } \
    } while(0)

#define MR_LOG_TERMINATOR "\0"

#define MR_PRINTF(module_idx, level, msg, ...) \
    do {\
        mr_log_sprintf((module_idx), (level), __func__, __FILE__, __LINE__, msg , ##__VA_ARGS__, MR_LOG_TERMINATOR);\
        _log_flush(module_idx);\
    }while(0) 

// 
// MR LOG
//
#define MR_LOG(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_CRIT(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_CRIT, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_ERR(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_ERR, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_WARN(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_WARN, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_INFO(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_DBG(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_DBG, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_MEM(msg, ...) \
    do{\
        MR_PRINTF(LOG_DEFAULT_MODULE_INDEX, L_MEM, msg, ##__VA_ARGS__);\
    } while(0)

//
// MR log Module based
//
#define MR_LOG_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module(module_name);\
        if (log_mod_idx > 0) MR_PRINTF(log_mod_idx, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_CRIT_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module(module_name);\
        if (log_mod_idx > 0) MR_PRINTF(log_mod_idx, L_CRIT, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_ERR_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module(module_name);\
        if (log_mod_idx > 0) MR_PRINTF(log_mod_idx, L_ERR, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_WARN_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module(module_name);\
        if (log_mod_idx > 0) MR_PRINTF(log_mod_idx, L_WARN, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_INFO_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module(module_name);\
        if (log_mod_idx > 0) MR_PRINTF(log_mod_idx, L_INFO, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_DBG_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module_AT(module_name);\
        if (log_mod_idx > 0) MR_PRINTF(log_mod_idx, L_DBG, msg, ##__VA_ARGS__);\
    } while(0)

#define MR_LOG_MEM_AT(module_name, msg, ...) \
    do{\
        int log_mod_idx = log_find_module_AT(module_name);\
        MR_PRINTF(log_mod_idx, L_MEM, msg, ##__VA_ARGS__);\
    } while(0)

