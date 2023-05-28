cmake_minimum_required(VERSION 3.22)
project(vv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
find_package(PostgreSQL REQUIRED)
#target_include_directories(server PRIVATE ${PostgreSQL_INCLUDE_DIRS})
#target_include_directories(include PRIVATE ${PostgreSQL_INCLUDE_DIRS})

add_executable(server myServer/server-main.cpp)
target_link_libraries(server PRIVATE ${PostgreSQL_LIBRARIES})

#target_include_directories(server PRIVATE source/postgresql)
#target_link_libraries(server ${CMAKE_CURRENT_SOURCE_DIR}/source/postgresql -lpq)
add_executable(client myClient/client-main.cpp)
target_link_libraries(client PRIVATE ${PostgreSQL_LIBRARIES})
