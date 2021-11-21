#include <gtest/gtest.h>

extern "C" {

#include "log/log.h"
#include "log/log.h"
}

class TestLog: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("log-test", L_INFO);
        }

        // void TearDown() override {}
};

//TODO
TEST_F(TestLog, TestCreate) {
}
