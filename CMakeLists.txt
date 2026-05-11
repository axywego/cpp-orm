# cmake_minimum_required(VERSION 3.14)
# project(cpp-orm VERSION 1.0.0 LANGUAGES CXX)

# add_library(cpp-orm INTERFACE)
# add_library(cpp-orm::cpp-orm ALIAS cpp-orm)

# target_include_directories(cpp-orm INTERFACE include)

cmake_minimum_required(VERSION 3.14)
project(cpp-orm VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    libpqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx.git
    GIT_TAG 8.0.1
)

FetchContent_MakeAvailable(libpqxx)

add_library(cpp-orm INTERFACE)
add_library(cpp-orm::cpp-orm ALIAS cpp-orm)

target_include_directories(cpp-orm INTERFACE 
    include
)

target_link_libraries(cpp-orm INTERFACE pqxx)

option(ORM_DEBUG "Enable SQL debug output" OFF)

# # optional
# target_compile_definitions(cpp-orm INTERFACE ORM_DEBUG)

# option(BUILD_EXAMPLES "Build examples" ON)

# if(BUILD_EXAMPLES)
#     add_executable(example examples/main.cpp)
#     target_link_libraries(example PRIVATE cpp-orm::cpp-orm)
# endif()