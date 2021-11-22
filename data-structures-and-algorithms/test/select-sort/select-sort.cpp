#include <gtest/gtest.h>

extern "C" {

#include "select-sort/select-sort.h"
#include "log/log.h"
}

class TestSelectSort: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("select-sort-test", L_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestSelectSort, TestSort) {
    //TODO
}
