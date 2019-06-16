cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION &{CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

add_executable(allocator_app main.cpp)

target_compile_features(allocator_app PUBLIC cxx_std_17)

target_link_libraries(allocator_app PUBLIC allocators)