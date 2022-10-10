# engine/tests/unit/string/utf8/CMakeLists.txt

add_executable(code_unit_test
    "${CMAKE_CURRENT_SOURCE_DIR}/code_unit_test.cpp"
)

# Link gtest and set target settings
prep_target_for_test(code_unit_test)

gtest_add_tests(TARGET code_unit_test)
