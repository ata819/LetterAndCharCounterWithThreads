cmake_minimum_required(VERSION 3.16)
project(prog2 C)

set(CMAKE_C_STANDARD 99)

add_executable(prog2 main.c alphabetcountmulthreads.c testmulthreads.c count.h)