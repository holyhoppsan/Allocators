cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION &{CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

add_library(allocators STATIC Allocator.cpp 
                       FreeListAllocator.cpp
                       LinearAllocator.cpp
                       StackAllocator.cpp)
                       
target_compile_features(allocators PUBLIC cxx_std_17)

target_include_directories(allocators PUBLIC ${CMAKE_SOURCE_DIR}/include/allocators)