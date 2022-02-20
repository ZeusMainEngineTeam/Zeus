# engine/tests/unit/memory/endian/CMakeLists.txt

add_executable(endian_test endian_test.cpp)

# Link gtest and set target settings
prep_target_for_test(endian_test)

gtest_add_tests(TARGET endian_test)
