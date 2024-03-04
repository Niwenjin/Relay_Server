# 设置cmake最小版本
cmake_minimum_required(VERSION 3.6)

# 设置项目名称
project(Unix_Network_Programming)

# 设置C++标准11
set(CMAKE_CXX_STANDARD 11)

# 设置输出compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include_directories(include)

# link_directories(/app/json /app/lib)

set(SOURCE_FILES_TCPSERVER 
    src/mainreactor.cpp
    src/subreactor.cpp
    src/TCPserver.cpp
)

add_library(TCPserver STATIC ${SOURCE_FILES_TCPSERVER})

add_executable(server server.cpp)

target_link_libraries(server TCPserver)

add_library(TCPclient STATIC src/TCPclient.cpp)

add_executable(client client.cpp)

target_link_libraries(client TCPclient)
