cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION &{CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

file(GLOB test_source_files
    "*.cpp"
)

add_executable(unit_tests ${test_source_files})

target_link_libraries(unit_tests gtest_main allocators)

add_compile_definitions(WITH_TESTING)