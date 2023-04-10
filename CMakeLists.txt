cmake_minimum_required(VERSION 3.26)
project(ds)

add_executable(ds main.cpp)
install(TARGETS ds DESTINATION /bin)