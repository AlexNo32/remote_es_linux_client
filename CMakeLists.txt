cmake_minimum_required(VERSION 3.14)
project(client C)

set(CMAKE_C_STANDARD 99)

add_executable(client client.c csocket.c header.h operation.c tools.c)