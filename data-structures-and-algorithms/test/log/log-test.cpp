#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdarg.h>

extern "C" {
#include "log/log.h"
#include "log/log.h"
}

void *thread_func(void *arg)
{
    const char *str = (const char*) arg;
    LOG_CREATE_DEFAULT("log-test", L_INFO);

    if (!strcmp(str, "thread 1")) {
        sleep(1.5);
    }

    LOG_AT("log-test", "log message: %s", str);

    return NULL;
}

#define MAX_FAILURE_MESSAGE_SIZE 10 * DEFAULT_BUFFER_SIZE
#define VERIFICATION_BUFFER_SIZE 5 * DEFAULT_BUFFER_SIZE

class TestLog: public ::testing::Test {
    protected:
        void SetUp() override
        {
            m_old_stdout = dup(STDOUT_FILENO);
            ASSERT_TRUE(0 == pipe2(m_pipe, O_NONBLOCK));
            ASSERT_TRUE(STDOUT_FILENO == dup2(m_pipe[1], STDOUT_FILENO));
            close(m_pipe[1]);
        }

        void TearDown() override
        {
            ASSERT_TRUE(STDOUT_FILENO == dup2(m_old_stdout, STDOUT_FILENO));
            printf("%s", m_failure_msg);
        }

        int read_pipe(char *buf) {
            fflush(stdout);
            int len = read(m_pipe[0], buf, DEFAULT_BUFFER_SIZE);
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

            if (read_bytes != DEFAULT_BUFFER_SIZE-1) {
                p += sprintf(m_failure_msg + p,
                            "Failed comparing strings. Function: %s, line: %d. ",
                            func,
                            line);
                p += sprintf(m_failure_msg + p,
                            "read byes %d doesn't match DEFAULT_BUFFER_SIZE: %d.\n",
                            read_bytes,
                            DEFAULT_BUFFER_SIZE);
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
                            "Failed comparing strings. Function: %s, line: %d. ",
                            func,
                            line);
                p += sprintf(m_failure_msg + p,
                            "Actual: %s, expected: %s\n",
                            read_buffer,
                            verification_buffer);
                return 1;
            }

            return 0;
        }

        int assert_mr_log(
                 const char* func,
                 int line,
                 int labels_num,
                 const char *fmt, ...)
        {
            int read_bytes = 0, n = 0, p = 0;
            char read_buffer[VERIFICATION_BUFFER_SIZE], verification_buffer[VERIFICATION_BUFFER_SIZE];

            read_bytes = read_pipe(read_buffer);
            (void)read_bytes; //TODO: do i really need this?

            va_list args;
            va_start(args, fmt);
            for (int i=0; i<labels_num; i++) {
                char *label = va_arg(args, char*);
                n += snprintf(verification_buffer + n, VERIFICATION_BUFFER_SIZE - n, ",%s", label);
            }
            va_end(args);

            if (strstr(read_buffer, verification_buffer) == NULL) {
                p += sprintf(m_failure_msg + p,
                            "Failed comparing strings. Function: %s, line: %d. ",
                            func,
                            line);
                p += sprintf(m_failure_msg + p,
                            "Actual: %s, expected: %s\n",
                            read_buffer,
                            verification_buffer);
                return 1;
            }

            return 0;
        }

        int m_pipe[2];
        int m_old_stdout;
        char m_failure_msg[MAX_FAILURE_MESSAGE_SIZE];
};

#define err_quit(msg, error) \
    do { \
        printf(msg, error); \
        exit(1); \
    } while(0) \

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

#define ASSERT_LOG(fmt, ...) \
    LOG(fmt, ##__VA_ARGS__); \
    ASSERT_TRUE(assert_log(__func__, __LINE__, fmt, ##__VA_ARGS__) == 0)

TEST_F(TestLog, TestSprintf) {
    char *buf = (char*)malloc(2 * DEFAULT_BUFFER_SIZE * sizeof(char));
    memset((void*)buf, 0x51, 2 * DEFAULT_BUFFER_SIZE);

    log_create("unit-test-test-sprintf", "main");
    LOG_ADD_MODULE("test-sprintf", 1, L_INFO, NULL);

    LOG("%s", buf);
    assert_log_overflow(__func__, __LINE__);

    const char *a_test_message = "a test message";
    ASSERT_LOG("This is %s", a_test_message);
}

#define ASSERT_MR_LOG(msg, labels_num, ...) \
    MR_LOG(msg, ##__VA_ARGS__); \
    ASSERT_TRUE(assert_mr_log(__func__, __LINE__, labels_num, msg, ##__VA_ARGS__) == 0)

TEST_F(TestLog, TestMrLog) {
    char *buf = (char*)malloc(2 * DEFAULT_BUFFER_SIZE * sizeof(char));
    memset((void*)buf, 0x51, 2 * DEFAULT_BUFFER_SIZE);

    log_create("unit-test", "main");
    LOG_ADD_MODULE("test-mr-log", 1, L_INFO, NULL);

    MR_LOG("test log", buf);
    assert_log_overflow(__func__, __LINE__);

    ASSERT_MR_LOG("test log", 1, "\"label1\": \"val1\"");
}
