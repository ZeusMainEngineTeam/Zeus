# engine/tests/unit/math/vector/CMakeLists.txt

add_executable(matrix_test matrix_test.cpp)

# Link gtest and set target settings
prep_target_for_test(matrix_test)

gtest_add_tests(TARGET matrix_test)
