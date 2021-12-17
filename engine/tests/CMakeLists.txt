# engine/tests/CMakeLists.txt
# 
# Manages unit tests and integration tests

################################################################################
#                                                                              #
# Add GoogleTest To Project                                                    #
#                                                                              #
################################################################################

# Code below from: 
#   https://github.com/google/googletest/blob/main/googletest/README.md
#
# NOTE: C++11 is required to build Google Test

include(FetchContent)
FetchContent_Declare(googletest
  # Specify the commit you depend on and update it regularly.
  # NOTE: Currently set to Google Test 1.11.0 (Commit on Jun 11, 2021)
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Adds functions to interact with GoogleTest
include(GoogleTest)

################################################################################
#                                                                              #
# Add Tests                                                                    #
#                                                                              #
################################################################################

# Add wrapper for GoogleTest
include(PrepTargetForTest)

if(ZEUS_BUILD_UNIT_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/unit")
endif()

if(ZEUS_BUILD_INTEGRATION_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/integration")
endif()
