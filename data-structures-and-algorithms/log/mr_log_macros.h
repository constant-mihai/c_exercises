#pragma once

#include "log.h"

#define MR_LOG_END() \
    do {\
        _mr_log_flush(); \
    }while(0)

#define MR_LOG(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        L_INFO, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_CRIT(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        L_CRIT, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_ERR(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        L_ERR, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_WARN(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        L_WARN, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_INFO(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        L_INFO, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_DBG(msg) \
    do{\
        mr_log_preamble(LOG_DEFAULT_MODULE_INDEX, \
                        L_DBG, \
                        __func__, __FILE__, __LINE__, \
                        (msg)); \
    } while(0)

#define MR_LOG_MEM(msg) \
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
                            L_INFO, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_CRIT_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            L_INFO, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_ERR_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            L_ERR, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_WARN_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            L_WARN, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_INFO_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            L_INFO, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_DBG_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            L_DBG, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

#define MR_LOG_MEM_AT(log_mod_idx, msg) \
    do{\
        if (log_mod_idx >= 0) { \
            mr_log_preamble((log_mod_idx), \
                            L_MEM, \
                            __func__, __FILE__, __LINE__, \
                            (msg)); \
        }\
    } while(0)

