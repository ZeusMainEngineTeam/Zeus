# engine/docs/CMakeLists.txt

configure_file(Doxyfile.in Doxyfile)

# Add command that builds documentation using Doxygen
add_custom_target(Doxygen
    COMMAND
        doxygen "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
)

add_dependencies(Doxygen Zeus)
