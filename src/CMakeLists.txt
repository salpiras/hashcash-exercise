cmake_minimum_required(VERSION 3.9)
project(hashcash)

set(CMAKE_CXX_STANDARD 14)

add_executable(hashcash main.cpp md5.cpp md5.h hashcash.h hashcash.cpp IHasher.h MD5Hasher.cpp MD5Hasher.h)