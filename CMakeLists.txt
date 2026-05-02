cmake_minimum_required(VERSION 3.14)
project(cpp-orm VERSION 1.0.0 LANGUAGES CXX)

add_library(cpp-orm INTERFACE)
add_library(cpp-orm::cpp-orm ALIAS cpp-orm)

target_include_directories(cpp-orm INTERFACE include)