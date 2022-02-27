# CMakeLists.txt

# cmake_dependent_option requires 3.22
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Zeus
    DESCRIPTION "A cross-platform 3D game engine written in modern C++17 from scratch."
    LANGUAGES CXX
    VERSION 1.0.0
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
    message(WARNING "Build type is defaulted to Debug.")
endif()

################################################################################
#                                                                              #
# Zeus Game Engine Build Options                                               #
#                                                                              #
################################################################################

# Some options depend on previous options
include(CMakeDependentOption)

option(ZEUS_BUILD_DOCS "Builds docs for Zeus." ON)

option(ZEUS_BUILD_EXAMPLES "Builds examples for Zeus." ON)
cmake_dependent_option(ZEUS_ENABLE_VALGRIND_ON_EXAMPLES "Sets up Valgrind for code examples for Zeus." ON "ZEUS_BUILD_EXAMPLES" OFF)

option(ZEUS_BUILD_TESTS "Builds tests for Zeus." ON)
cmake_dependent_option(ZEUS_BUILD_UNIT_TESTS "Builds unit tests for Zeus." ON "ZEUS_BUILD_TESTS" OFF)
cmake_dependent_option(ZEUS_ENABLE_COVERAGE_ON_UNIT_TESTS "Enables code coverage on unit tests." ON "ZEUS_BUILD_TESTS;ZEUS_BUILD_UNIT_TESTS" OFF)

cmake_dependent_option(ZEUS_BUILD_INTEGRATION_TESTS "Builds integration tests for Zeus." ON "ZEUS_BUILD_TESTS" OFF)

cmake_dependent_option(ZEUS_ENABLE_VALGRIND_ON_TESTS "Sets up Valgrind for unit and integration tests for Zeus." ON "ZEUS_BUILD_TESTS" OFF)

option(ZEUS_DEBUGGING "Turn on debugging in Zeus." ON)

option(ZEUS_ENABLE_CLANG_TIDY "Turn on clang-tidy in Zeus." OFF)

# Runs Google C++ style guide checker before project is built
option(ZEUS_ENABLE_STYLE_CHECK "Enables C++ style checking." ON)

# Sanitizers to run before the project is built.
option(ZEUS_ENABLE_SANITIZERS "Enables sanitizers to run before build." ON)
cmake_dependent_option(ZEUS_ENABLE_THREAD_SANITIZER "Enables thread sanitizer for detecting data races and deadlocks in Zeus." ON "ZEUS_ENABLE_SANITIZERS" OFF)
cmake_dependent_option(ZEUS_ENABLE_ADDRESS_SANITIZER "Enables address sanitizer for detecting addressability issues in Zeus." ON "ZEUS_ENABLE_SANITIZERS" OFF)
cmake_dependent_option(ZEUS_ENABLE_MEMORY_SANITIZER "Enables memory sanitizer for detecting use of uninitialized memory in Zeus." ON "ZEUS_ENABLE_SANITIZERS" OFF)
cmake_dependent_option(ZEUS_ENABLE_LEAK_SANITIZER "Enables leak sanitizer for detecting memory leaks in Zeus." ON "ZEUS_ENABLE_SANITIZERS" OFF)

################################################################################
#                                                                              #
# Add directories for project based on previous build options                  #
#                                                                              #
################################################################################

# Check for PowerShell since a lot of cross-platform commands and scripts are 
# written in PowerShell
find_program(PWSH NAMES pwsh powershell)

if(NOT PWSH)
    message(WARNING "Could not find PowerShell. A lot of custom targets will not be built!")
endif()

if(ZEUS_BUILD_TESTS)
    # Allows for running test from the root of the build directory
    # 'make test' to run all tests
    enable_testing()

    if(ZEUS_ENABLE_VALGRIND_ON_TESTS AND PWSH)
        # Enables ctest -T memcheck (automatically locates valgrind)
        # Generates DartConfiguration file needed to run
        include(CTest)

        # Create custom command to run ctest -T memcheck and print out valgrind results for unit tests
        add_custom_target(unit_test_memcheck
            # Start in binary directory of project
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"

            # Run ctest -T memcheck
            COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck

            # Print out log files written to by CTest
            # Runs the PowerShell command equivalent to Linux's cat
            #
            # Note: MemoryChecker.<#>.log where '#' is the unit test number
            COMMAND ${PWSH} --Command cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
        )
    endif()
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")
