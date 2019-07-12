cmake_minimum_required(VERSION 3.14)
project(client.c C)

set(CMAKE_C_STANDARD 99)

add_executable(client client.c client.h cSocket.c cSocket.h)