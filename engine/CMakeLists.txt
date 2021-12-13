# engine/CMakeLists.txt

# Add our cmake files to the module path for the project
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

message(STATUS "${CMAKE_MODULE_PATH}")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

if(ZEUS_BUILD_EXAMPLES)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif()

if(ZEUS_BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()

if(ZEUS_BUILD_DOCS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/docs")
endif()
