#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    std::cout << "TEST_DIR '" << TEST_DIR << "'" << std::endl;
    for (int i = 1; i < argc; ++i) {
        std::cout << "Input[" << i << "]: "<< argv[i] << std::endl;
    }
// another  comment to add and push to see actions
    return RUN_ALL_TESTS();
}