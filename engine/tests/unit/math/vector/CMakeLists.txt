# engine/tests/unit/math/vector/CMakeLists.txt

add_executable(vector_test vector_test.cpp)

# Link gtest and set target settings
prep_target_for_test(vector_test)

gtest_add_tests(TARGET vector_test)
