cmake_minimum_required(VERSION 3.20)
project(SCAPMS LANGUAGES CXX)

if(NOT WIN32)
  message(FATAL_ERROR "SCAPMS currently uses the native Windows API. Build on Windows with a C++ desktop toolchain.")
endif()

add_executable(scapms WIN32
  src/main_win.cpp
  src/core.cpp
)
target_include_directories(scapms PRIVATE include)
target_compile_features(scapms PRIVATE cxx_std_17)
target_compile_definitions(scapms PRIVATE NOMINMAX)
target_link_libraries(scapms PRIVATE comctl32 comdlg32)
if(MSVC)
  target_compile_options(scapms PRIVATE /W4 /permissive- /EHsc)
  target_link_options(scapms PRIVATE "/MANIFEST:EMBED" "/MANIFESTINPUT:${CMAKE_CURRENT_SOURCE_DIR}/app.manifest")
endif()
