cmake_minimum_required(VERSION 2.8)
project(sdy_bot)
set(Boost_USE_MULTITHREADERD ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -std=c++1z")

add_executable(sdybot 
    src/slackbotpp.cpp
    src/slackbot.cpp
    src/slackmessages.cpp)
find_package(Boost 1.62.0)
target_link_libraries(sdybot boost_system boost_thread  yaml-cpp pthread ssl crypto)
