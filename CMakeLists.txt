cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 11)

project(wxfractal CXX)

set(
    sources
    src/main.cpp
    src/core/turtle.cpp
    src/gui/main_gui.cpp
    src/gui/main_frame.cpp
    src/gui/draw_panel.cpp
    src/gui/controller.cpp
    src/core/koch.cpp
)

# use dynamic wx
# add_compile_definitions(WXUSINGDLL)

# use static wx
set(wxBUILD_SHARED OFF CACHE BOOL "use static wx" FORCE)

add_subdirectory(vendor/wxWidgets)
# add_executable(wxfractal WIN32 ${sources})

# if(MSVC)
# 	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
# endif()

add_executable(wxfractal ${sources})
target_link_libraries(wxfractal wx::core wx::base wx::net)

# set_property(TARGET wxfractal PROPERTY
#   MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# set_property(TARGET wxfractal PROPERTY
#   MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")