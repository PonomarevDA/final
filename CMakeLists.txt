cmake_minimum_required(VERSION 2.8)

# set the project name
project(final)

# add libraries
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS}) 

# add the headers into the project
include_directories(include)

# add src files
set(SRC_LIST src/server.cpp src/main.cpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# Link library
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})