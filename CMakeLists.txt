project(Python-Cplus-Rpc)
cmake_minimum_required(VERSION 2.8)

# -std=c++11
add_definitions( -DZMQ_STATIC)

add_subdirectory(deps/zeromq-4.2.1)

include_directories(
    deps/zeromq-4.2.1/include
    cplus/inc
    deps/json
)

# server app
set(CPLUS_SERVER
    cplus/src/cplus_server.cpp
    test/server.cpp
)

add_executable(CplusRpcServer ${CPLUS_SERVER})
target_link_libraries(CplusRpcServer libzmq-static)
if (UNIX)
target_link_libraries(CplusRpcServer pthread)
endif()
