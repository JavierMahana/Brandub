cmake_minimum_required(VERSION 3.25)
project(Brandub)

set(CMAKE_CXX_STANDARD 17)

add_executable(Brandub main.cpp Bitboard.cpp Bitboard.h Move.cpp Move.h Game.cpp Game.h)
