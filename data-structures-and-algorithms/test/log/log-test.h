#pragma once

#define err_quit(msg, error) \
    do { \
        printf(msg, error); \
        exit(1); \
    } while(0) \

#define ASSERT_LOG(fmt, ...) \
    LOG(fmt, ##__VA_ARGS__); \
    ASSERT_TRUE(assert_log(__func__, __LINE__, fmt, ##__VA_ARGS__) == 0)

#define ASSERT_MR_LOG(lvl, expected_str, msg, body) \
    MR_LOG##lvl(msg); \
    do { body; } while (0);\
    MR_LOG_END();\
    ASSERT_TRUE(assert_mr_log(__func__, __LINE__, expected_str) == 0)

#define ASSERT_MR_LOG_AT(lvl, module_idx, expected_str, msg, body) \
    MR_LOG##lvl(module_idx, msg); \
    do { body; } while (0);\
    MR_LOG_END();\
    ASSERT_TRUE(assert_mr_log(__func__, __LINE__, expected_str) == 0)
