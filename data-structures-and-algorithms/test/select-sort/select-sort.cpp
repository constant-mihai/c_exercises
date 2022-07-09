#include <gtest/gtest.h>

extern "C" {

#include "select-sort/select-sort.h"
#include "log/log.h"
}

class TestSelectSort: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_CREATE_DEFAULT("select-sort-test", LOG_LEVEL_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestSelectSort, TestSort) {
    //TODO
}
