#include <gtest/gtest.h>
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

class TestLog: public ::testing::Test {
    protected:
        void SetUp() override {
        }

        // void TearDown() override {}
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

TEST_F(TestLog, TestMrLog) {
    LOG_CREATE_DEFAULT("log-test", L_INFO);
    //TODO re-write this after implementing encoders
    MR_LOG("test log", "\"label1\": \"val1\"");
}
