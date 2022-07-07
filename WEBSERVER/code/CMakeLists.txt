cmake_minimum_required(VERSION 3.0)
project(WEBSERVER)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread -o3")
set(SRC_LIST main.cpp buffer.cpp epoller.cpp HTTPconnection.cpp HTTPrequest.cpp
HTTPresponse.cpp log.cpp sqlconnpool.cpp timer.cpp webserver.cpp)
add_executable(webserver ${SRC_LIST})
target_link_libraries(webserver -lmysqlclient)