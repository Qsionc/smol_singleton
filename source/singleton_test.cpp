/**
 * This software is free to use.
 * Consider leaving a comment with some insight.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <SingletonWrapper.h>

using namespace singleton;

class TestSingletonWrapperFixture : public testing::Test {
public:
    ~TestSingletonWrapperFixture() override = default;

protected:
    static std::stringstream sout;
    static std::string delete_msg;

    template<class T>
    struct Deleter {
        void operator()(T *_ptr) {
            TestSingletonWrapperFixture::sout << TestSingletonWrapperFixture::delete_msg;
            delete _ptr;
        }
    };

    SingletonWrapper<int, Deleter<int>> intWrapper;

    void SetUp() override {
        sout.str(std::string());
        delete_msg = "Deleted Pointer";
        intWrapper.init(42);
    }

    void TearDown() override {
        intWrapper.destroy();
    }
};

std::stringstream TestSingletonWrapperFixture::sout;
std::string TestSingletonWrapperFixture::delete_msg;

TEST_F(TestSingletonWrapperFixture, creation_test) {
    ASSERT_EQ(*intWrapper, 42);
}

TEST_F(TestSingletonWrapperFixture, multiple_instances_test) {
    SingletonWrapper<int, Deleter < int>>
    ref1 = intWrapper;
    SingletonWrapper<int, Deleter < int>>
    ref2(intWrapper);
    ASSERT_EQ(&(*ref1), &(*ref2));
}

TEST_F(TestSingletonWrapperFixture, destruction_test) {
    intWrapper.destroy();
    ASSERT_EQ(sout.str(), delete_msg);
}

TEST_F(TestSingletonWrapperFixture, destruction_empty_singleton_test) {
    intWrapper.destroy();
    ASSERT_FALSE(intWrapper);
}

TEST_F(TestSingletonWrapperFixture, scope_control_test) {
    {
        SingletonWrapper<int, Deleter < int>> ref(intWrapper);
        (*ref) = 30;
    }
    ASSERT_EQ(*intWrapper, 30);
    ASSERT_TRUE(intWrapper);
}

int main(int _argc, char *_argv[]) {
    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}
