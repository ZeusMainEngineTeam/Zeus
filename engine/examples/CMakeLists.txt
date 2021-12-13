# engine/examples/CMakeLists.txt

# The purpose of this CMakeLists file is to organize
# the game engine's modules more easily and to allow
# the tests to expand easier

include(AddZeusExample)


# Add modules
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/math")
