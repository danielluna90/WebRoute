link_libraries(WebRoute)
include_directories("${WEBROUTE_SOURCE_DIR}/include")

add_executable(http-server src/main.cpp)
