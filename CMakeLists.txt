cmake_minimum_required(VERSION 3.14)
project(client C)

set(CMAKE_C_STANDARD 99)

add_executable(client client.c cSocket.c cSocket.h operation.c tools.c)