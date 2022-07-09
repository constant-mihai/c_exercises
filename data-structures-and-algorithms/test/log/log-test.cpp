#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdarg.h>

#include "log-test.h"

extern "C" {
#include "log/log.h"
#include "log/log.h"
}

void *thread_func(void *arg)
{
    const char *str = (const char*) arg;
    LOG_CREATE_DEFAULT("log-test", LOG_LEVEL_INFO);

    if (!strcmp(str, "thread 1")) {
        sleep(1.5);
    }

    int log_test_idx = log_find_module("log-test");
    assert(log_test_idx == 0);
    LOG_AT(log_test_idx, "log message: %s", str);

    log_destroy();

    return NULL;
}

#define MAX_FAILURE_MESSAGE_SIZE 15 * DEFAULT_BUFFER_SIZE
#define VERIFICATION_BUFFER_SIZE 5 * DEFAULT_BUFFER_SIZE

class TestLog: public ::testing::Test {
    public:
        TestLog():
            m_pipe{0, 0},
            m_old_stdout(0),
            m_failure_msg{}
        {};

    //~TestLog() {};

    protected:
        void SetUp() override
        {
            log_create("test-log", "main");
            LOG_ADD_MODULE("default", 1, LOG_LEVEL_INFO, NULL);

            m_old_stdout = dup(STDOUT_FILENO);
            ASSERT_TRUE(0 == pipe2(m_pipe, O_NONBLOCK));
            ASSERT_TRUE(STDOUT_FILENO == dup2(m_pipe[1], STDOUT_FILENO));
            close(m_pipe[1]);
        }

        void TearDown() override
        {
            ASSERT_TRUE(STDOUT_FILENO == dup2(m_old_stdout, STDOUT_FILENO));
            printf("%.*s", MAX_FAILURE_MESSAGE_SIZE, m_failure_msg);

            log_destroy();
        }

        int read_pipe(char *buf) {
            fflush(stdout);
            int len = read(m_pipe[0], buf, VERIFICATION_BUFFER_SIZE);
            if (len < 0) {
                int error = errno;
                EXPECT_TRUE(EAGAIN == error); // 0 byte is read
                buf = 0;
                len = 0;
            }
            return len;
        }

        int assert_log_overflow(
                 const char* func,
                 int line)
        {
            int read_bytes = 0, p = 0;
            char read_buffer[VERIFICATION_BUFFER_SIZE];

            read_bytes = read_pipe(read_buffer);

            // TODO: for some reason read_bytes doesn't count \0
            if (read_bytes != DEFAULT_BUFFER_SIZE-1) {
                p += sprintf(m_failure_msg + p,
                            "Failed comparing strings. Function: %s, line: %d. ",
                            func,
                            line);
                p += sprintf(m_failure_msg + p,
                            "read byes %d doesn't match DEFAULT_BUFFER_SIZE: %d.\n",
                            read_bytes,
                            DEFAULT_BUFFER_SIZE);
                p += sprintf(m_failure_msg + p,
                            "read string: %.*s.\n",
                            read_bytes,
                            read_buffer);
                return 1;
            }
            return 0;
        }

        int assert_log(
                 const char* func,
                 int line,
                 const char *fmt, ...)
        {
            int read_bytes = 0, n = 0, p = 0;
            char read_buffer[VERIFICATION_BUFFER_SIZE], verification_buffer[VERIFICATION_BUFFER_SIZE];

            read_bytes = read_pipe(read_buffer);
            (void)read_bytes; //TODO: do i really need this?

            va_list args;
            va_start(args, fmt);
            n = vsnprintf(verification_buffer, VERIFICATION_BUFFER_SIZE, fmt, args);
            (void)n; //TODO: do i really need this?
            va_end(args);

            if (strstr(read_buffer, verification_buffer) == NULL) {
                p += sprintf(m_failure_msg + p,
                            "Log test failed. Function: %s, line: %d. ",
                            func,
                            line);
                p += sprintf(m_failure_msg + p,
                            "Actual log message: %s, Expected substring: %s\n",
                            read_buffer,
                            verification_buffer);
                return 1;
            }

            return 0;
        }

        int assert_mr_log(
                 const char* func,
                 int line,
                 const char* expected_str)
        {
            int read_bytes = 0, p = 0;
            char read_buffer[VERIFICATION_BUFFER_SIZE];

            read_bytes = read_pipe(read_buffer);
            (void)read_bytes; //TODO: do i really need this?

            if (strstr(read_buffer, expected_str) == NULL) {
                p += sprintf(m_failure_msg + p,
                            "Log test failed. Function: %s, line: %d. ",
                            func,
                            line);
                p += sprintf(m_failure_msg + p,
                            "Actual log message: %s, Expected substring: %s\n",
                            read_buffer,
                            expected_str);
                return 1;
            }

            return 0;
        }

        int m_pipe[2];
        int m_old_stdout;
        char m_failure_msg[MAX_FAILURE_MESSAGE_SIZE];
};

TEST_F(TestLog, TestCreate) {
    pthread_t thread_ids[2];
    void *thread_result;
    int status;

    const char *str1 = "thread 1";
    const char *str2 = "thread 2";
    status = pthread_create ( &thread_ids[0], NULL, thread_func, (void*)str1);
    if (status != 0) err_quit ("thread 0 %s", strerror (status));

    status = pthread_create ( &thread_ids[1], NULL, thread_func, (void*)str2);
    if (status != 0) err_quit ("thread 1 %s", strerror (status));

    status = pthread_join (thread_ids[0], &thread_result);
    if (status != 0) err_quit ("Join thread %s", strerror(status));

    status = pthread_join (thread_ids[1], &thread_result);
    if (status != 0) err_quit ("Join thread %s", strerror(status));
}

TEST_F(TestLog, TestSprintf) {
    char *buf = (char*)malloc(2 * DEFAULT_BUFFER_SIZE * sizeof(char));
    memset((void*)buf, 0x51, 2 * DEFAULT_BUFFER_SIZE);

    LOG("%.*s", 2*DEFAULT_BUFFER_SIZE, buf);
    ASSERT_TRUE(0 == assert_log_overflow(__func__, __LINE__));

    const char *a_test_message = "a test message";
    ASSERT_LOG("This is %s", a_test_message);

    free(buf);
}

TEST_F(TestLog, TestLogfile) {
    const char* filename = "file-mod.log";
    log_config_t log_config = {
        .log_to_console = 0,
        .level = LOG_LEVEL_INFO,
        .filename = (char*)filename
    };
    log_add_module("file-mod", log_config);

    const char *a_test_message = "a test message";
    LOG_AT(log_find_module("file-mod"), "This is %s", a_test_message);
    LOG_AT(log_find_module("file-mod"), "This is %s", a_test_message);
    LOG_AT(log_find_module("file-mod"), "This is %s", a_test_message);

    MR_LOG_AT(log_find_module("file-mod"), "This is");
    mr_log_string("value", a_test_message);
    MR_LOG_END();
}

TEST_F(TestLog, TestReallocModule) {
    const char* mod_name = "mod-name";
    char full_name[128];
    for (int i=0; i<2*DEFAULT_MODULES_NUM; i++) {
        log_config_t log_config = {
            .log_to_console = 1,
            .level = LOG_LEVEL_INFO,
            .filename = 0 
        };
        sprintf(full_name, "%s-%d", mod_name, i+1);
        log_add_module(full_name, log_config);
    }

    const char *a_test_message = "a test message";
    LOG_AT(log_find_module("file-mod"), "This is %s", a_test_message);
    LOG_AT(log_find_module("file-mod"), "This is %s", a_test_message);
    LOG_AT(log_find_module("file-mod"), "This is %s", a_test_message);

    MR_LOG_AT(log_find_module("file-mod"), "This is");
    mr_log_string("value", a_test_message);
    MR_LOG_END();
}


TEST_F(TestLog, TestMrLog) {
    char *buf = (char*)malloc(2 * DEFAULT_BUFFER_SIZE * sizeof(char));
    memset((void*)buf, 0x53, 2 * DEFAULT_BUFFER_SIZE);
    buf[(2*DEFAULT_BUFFER_SIZE)-1] = '\0';

    MR_LOG(buf);
    MR_LOG_END();

    ASSERT_TRUE(0 == assert_log_overflow(__func__, __LINE__));

    ASSERT_MR_LOG(_ERR,
                  "\"error\":\"error value\"",
                  "test log",
                  ({ mr_log_error("error value"); }));
    ASSERT_MR_LOG_AT(_ERR_AT,
                     LOG_DEFAULT_MODULE_INDEX,
                     "\"error\":\"error value\"",
                     "test log",
                     ({ mr_log_error("error value"); }));

    free(buf);
}
