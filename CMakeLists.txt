cmake_minimum_required(VERSION 3.27)
project(WebRoute)

# Options
option(WEBROUTE_COMPILE_EXAMPLE_APPLICATIONS "Build Example Applications" ON)
option(WEBROUTE_COMPILE_DOCUMENTATION "Build Documentation" ON)

# CMake / C++ Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# WebRoute
add_library(WebRoute src/log.cpp src/webroute.cpp src/http/parser.cpp)
target_include_directories(WebRoute PRIVATE include)

# Example Applications
if(WEBROUTE_COMPILE_EXAMPLE_APPLICATIONS)
  add_subdirectory(examples/http-server)
  # add_subdirectory(examples/https-server)
endif(WEBROUTE_COMPILE_EXAMPLE_APPLICATIONS)

# Documentation
if(WEBROUTE_COMPILE_DOCUMENTATION)
  add_subdirectory(docs)
endif(WEBROUTE_COMPILE_DOCUMENTATION)
