cmake_minimum_required(VERSION 3.20)
project(Project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files here
add_executable(MyExecutable
        plains25a1.cpp
        Horse.cpp
        Herd.cpp
        AVLNode.h
        AVL.h
        LinkedList.h)

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)
