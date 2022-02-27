function Build-Debug-Project {
    # Create 'build' directory in main project directory
    New-Item -Path "${PSScriptRoot}/../build" -ItemType Directory -force

    Write-Output "Calling CMake..."

    # Call CMake to build project
    # CMAKE_EXPORT_COMPILE_COMAMNDS is for clang-tidy
    Invoke-Expression -command "cmake -S ${PSScriptRoot}/.. -B ${PSScriptRoot}/../build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
}

Build-Debug-Project
