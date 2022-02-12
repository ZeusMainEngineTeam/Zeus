# engine/src/CMakeLists.txt

# Add our cmake files to the module path for the project
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

################################################################################
#                                                                              #
# Zeus Source Files                                                            #
#                                                                              #
################################################################################

add_executable(Zeus
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

# Simple function to add source files to the Zeus target
#include(AddZeusSource)

# Add source files in modules
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/core")

################################################################################
#                                                                              #
# Zeus Target Settings                                                         #
#                                                                              #
################################################################################

# Contains project information
configure_file("${PROJECT_SOURCE_DIR}/engine/cmake/config.hpp.in" "${PROJECT_SOURCE_DIR}/engine/include/zeus/config.hpp")

set_target_properties(Zeus
    PROPERTIES
        SUFFIX "_${CMAKE_CXX_COMPILER_ID}_${CMAKE_SYSTEM_NAME}_${Zeus_VERSION_MAJOR}_${Zeus_VERSION_MINOR}_${Zeus_VERSION_PATCH}_${CMAKE_BUILD_TYPE}${CMAKE_EXECUTABLE_SUFFIX}"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(ZEUS_ENABLE_CLANG_TIDY)
    message(STATUS "clang-tidy enabled for Zeus.")

    set_target_properties(Zeus
        PROPERTIES
            CXX_CLANG_TIDY "clang-tidy;-header-filter=.*;-checks=-*,-abseil*,-android*,-fuchsia-overloaded-operator,-fuchsia-trailing-return,-fuchsia-multiple-inheritance,bugprone-*,cert-*,cppcoreguidelines-*,clang-analyzer-cplusplus*,-clang-analyzer-osx*,misc-*,modernize-*,-modernize-use-trailing-return-type,performance-*,portability-*,readability-*,hicpp-*,llvm-*,-llvm-header-guard,-llvm-include-order,google-*;-warnings-as-errors=*"
    )
endif()

target_include_directories(Zeus
    PUBLIC
        "../include"
        "${PROJECT_BINARY_DIR}"
)

# Set environmental variables
target_compile_definitions(Zeus
    PRIVATE
        # Debugging
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ZEUS_DEBUG>
        $<$<CONFIG:Release>:ZEUS_NDEBUG>

        # Utility
        $<$<BOOL:${ZEUS_ENABLE_FORCE_INLINE}>:ZEUS_ENABLE_FORCE_INLINE>

        # Logging
        $<$<BOOL:${ZEUS_ENABLE_LOGGING}>:ZEUS_ENABLE_LOGGING>
        $<$<BOOL:${ZEUS_ENABLE_LOGGING}>:ZEUS_LOGGING_LEVEL=${ZEUS_LOGGING_LEVEL}>
)

target_compile_options(Zeus
    PRIVATE
        # Microsoft Visual C++ Compiler
        $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>

        # GCC, Clang etc.
        # Note: Only works for compilers that support these options.
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -Wall -Wextra -pedantic -Werror -Wfatal-errors>
)
