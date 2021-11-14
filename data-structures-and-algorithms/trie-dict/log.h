#ifndef __TRIE_DICT_LOG__H__
#define __TRIE_DICT_LOG__H__

#include <stdlib.h>

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

#endif
