cmake_minimum_required(VERSION 3.10)

# set the project name
project(oscoursework)

include_directories(infos/include)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(deneme2 SHARED ${SOURCES})

add_executable(
        deneme
        coursework/buddy.cpp
        coursework/cmos-rtc.cpp
        coursework/sched-rr.cpp
)
