# PrepTargetForTest.cmake

macro(PREP_TARGET_FOR_TEST arg_test_target)
    get_target_property(ZEUS_INCLUDES Zeus INCLUDE_DIRECTORIES)
    get_target_property(ZEUS_CXX_STANDARD Zeus CXX_STANDARD)
    get_target_property(ZEUS_SOURCES Zeus SOURCES)

    target_include_directories(${arg_test_target}
        PRIVATE
            "${ZEUS_INCLUDES}"
    )

    target_link_libraries(${arg_test_target}
        PRIVATE
            gtest_main
    )

    set_target_properties(${arg_test_target}
        PROPERTIES
            CXX_STANDARD ${ZEUS_CXX_STANDARD}
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )

    # Temp solution
    target_sources(${arg_test_target} PRIVATE "${ZEUS_SOURCES}")
endmacro()
