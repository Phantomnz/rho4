# 1. Set the minimum CMake version and project name
cmake_minimum_required(VERSION 3.15)
project(PID_Dashboard CXX) # "CXX" means it's a C++ project

# 2. Set the C++ standard (to match your -std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 3. Add your executable.
# This one command replaces the list of .cpp files in your Makefile.
add_executable(dashboard.exe
    main.cpp
    SerialPort.cpp
    
    # Add the ImGui source files
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# 4. Add the include directories.
# This replaces all your "-I..." CXXFLAGS.
target_include_directories(dashboard.exe PUBLIC
    # "PUBLIC" means "add this to the include path"
    imgui
    imgui/backends
    glfw/include
)

# 5. Add the library directory.
# This replaces your "-L..." LIBS flag.
target_link_directories(dashboard.exe PUBLIC
    glfw/lib-mingw-w64
)

# 6. Link the libraries.
# This replaces the "-l..." LIBS flags.
target_link_libraries(dashboard.exe PRIVATE
    glfw3       # from your -lglfw3
    opengl32    # from your -lopengl32
    gdi32       # from your -lgdi32
    imm32       # from your -limm32
)

# 7. (Optional but good practice) Add compiler flags.
# This adds the -Wall (all warnings) and -g (debug) flags.
target_compile_options(dashboard.exe PRIVATE -Wall -g)

# A small message to confirm it's working
message(STATUS "Configured PID_Dashboard project. Run 'mingw32-make' to build.")
