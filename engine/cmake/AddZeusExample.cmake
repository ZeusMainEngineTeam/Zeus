# AddZeusExample.cmake

# A simple wrapper to add examples to Zeus
macro(ADD_ZEUS_EXAMPLE arg_test_target)
    get_target_property(ZEUS_INCLUDES Zeus INCLUDE_DIRECTORIES)
    get_target_property(ZEUS_CXX_STANDARD Zeus CXX_STANDARD)

    target_include_directories(${arg_test_target}
        PUBLIC
            "${ZEUS_INCLUDES}"
    )

    set_target_properties(${arg_test_target}
        PROPERTIES
            CXX_STANDARD ${ZEUS_CXX_STANDARD}
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
endmacro()
