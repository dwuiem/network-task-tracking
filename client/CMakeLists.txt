cmake_minimum_required(VERSION 3.27)
project(client)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC networking)

target_link_libraries(${PROJECT_NAME} PUBLIC networking)