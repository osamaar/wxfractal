cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 11)

project(wxfractal CXX)

set(
    sources
    src/main.cpp
    src/main_gui.cpp
    src/main_frame.cpp
)

# use dynamic wx
# add_compile_definitions(WXUSINGDLL)

# use static wx
set(wxBUILD_SHARED OFF CACHE BOOL "use static wx" FORCE)

add_subdirectory(vendor/wxWidgets)
add_executable(wxfractal WIN32 ${sources})
target_link_libraries(wxfractal wx::core wx::base wx::net)
