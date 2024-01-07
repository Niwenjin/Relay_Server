# 设置cmake最小版本
cmake_minimum_required(VERSION 3.6)

# 设置项目名称
project(Unix_Network_Programming)

# 设置C++标准11
set(CMAKE_CXX_STANDARD 11)

# 设置输出compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# file(GLOB_RECURSE LIB_ABSL /usr/lib/libabsl_*.so)
# find_library(LIB_ABSL libabsl_*.* /usr/lib)
# find_package(grpc CONFIG REQUIRED)

include_directories(include)

# link_directories(/app/json /app/lib)

add_executable(server server.cpp src/socket.cpp)

add_executable(client client.cpp src/socket.cpp)

# target_link_libraries(server ${grpc_LIBRARIES} ${absl_LIBRARIES} ${protobuf_LIBRARIES} jsoncpp)

# target_link_libraries(server ${grpc_LIBS})