#pragma once

#include "log.h"

#define MR_LOG_END() \
    do {\
        _mr_log_flush(); \
    }while(0)

#define MR_LOG(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        LOG_LEVEL_INFO, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_FATAL(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        LOG_LEVEL_FATAL, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_ERROR(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        LOG_LEVEL_ERROR, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_WARN(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        LOG_LEVEL_WARN, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_INFO(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        LOG_LEVEL_INFO, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_DBG(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        LOG_LEVEL_DBG, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_TRACE(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        (level), \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

//
// MR log Module based
//
#define MR_LOG_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_INFO, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_FATAL_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_INFO, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_ERROR_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_ERROR, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_WARN_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_WARN, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_INFO_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_INFO, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_DEBUG_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_DEBUG, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_TRACE_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            LOG_LEVEL_TRACE, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

