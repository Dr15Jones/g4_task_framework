cmake_minimum_required(VERSION 3.15)
project(g4_task_framework VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard (required for std::optional and std::any)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set TBB location
list(APPEND CMAKE_PREFIX_PATH "/Users/cdj/src/cms/software/unify_project/externals/lib/cmake")

# Find TBB package
find_package(TBB REQUIRED)

# Create shared library
add_library(G4TaskFramework SHARED
    Event.hpp
    Module.hpp
    EventSource.hpp
    EventLoopController.hpp
    EventLoopController.cpp
    EventProcessor.hpp
    EventProcessor.cpp
)

# Link TBB to library
target_link_libraries(G4TaskFramework PUBLIC TBB::tbb)

# Set include directories for library
target_include_directories(G4TaskFramework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create executable
add_executable(g4_task_framework
    main.cc
)

# Link executable to library
target_link_libraries(g4_task_framework PRIVATE G4TaskFramework)


add_executable(test_g4_task_framework
    test_main.cc
)

# Link executable to library
target_link_libraries(test_g4_task_framework PRIVATE G4TaskFramework)
