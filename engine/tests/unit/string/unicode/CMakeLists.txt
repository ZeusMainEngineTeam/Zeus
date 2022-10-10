# engine/tests/unit/string/unicode/CMakeLists.txt

add_executable(code_point_test
    "${CMAKE_CURRENT_SOURCE_DIR}/code_point_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/code_points_test.cpp"
)

# Link gtest and set target settings
prep_target_for_test(code_point_test)

gtest_add_tests(TARGET code_point_test)
