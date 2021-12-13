# CMakeLists.txt

# cmake_dependent_option requires 3.22
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Zeus
    DESCRIPTION "A cross-platform 3D game engine written in modern C++17 from scratch."
    LANGUAGES CXX
    VERSION 1.0.0
)

# Add our cmake files to the module path for the project
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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

option(ZEUS_BUILD_EXAMPLES "Builds examples for Zeus." ON)
option(ZEUS_BUILD_DOCS "Builds docs for Zeus." ON)

option(ZEUS_BUILD_TESTS "Builds tests for Zeus." ON)
cmake_dependent_option(ZEUS_BUILD_UNIT_TESTS "Builds unit tests for Zeus." ON "ZEUS_BUILD_TESTS" OFF)
cmake_dependent_option(ZEUS_ENABLE_COVERAGE_ON_UNIT_TESTS "Enables code coverage on unit tests." ON "ZEUS_BUILD_TESTS;ZEUS_BUILD_UNIT_TESTS" OFF)

cmake_dependent_option(ZEUS_BUILD_INTEGRATION_TESTS "Builds integration tests for Zeus." ON "ZEUS_BUILD_TESTS" OFF)

cmake_dependent_option(ZEUS_ENABLE_VALGRIND_ON_TESTS "Sets up Valgrind for unit and integration tests for Zeus." ON "ZEUS_BUILD_TESTS" OFF)

option(ZEUS_DEBUGGING "Turn on debugging in Zeus." ON)

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

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")
