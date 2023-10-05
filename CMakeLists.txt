cmake_minimum_required(VERSION 3.5)

project(Feiteng LANGUAGES CXX)

include (cmake/utils.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Find Qt
find_package(Qt5 COMPONENTS Core Gui Widgets Sql SerialPort REQUIRED)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Find Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Set UI search path
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)  

# Add your source and header files
file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE PROJECT_UIS ${CMAKE_SOURCE_DIR}/ui/*.ui)

# Create a shared library
add_library(Feiteng SHARED
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
  ${PROJECT_UIS}
)

force_redefine_file_macro_for_sources(Feiteng) # __FILE__

# Link libraries
target_link_libraries(Feiteng PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::SerialPort ${OpenCV_LIBS} yaml-cpp pthread)

# Include directories
target_include_directories(Feiteng PRIVATE 
    ${CMAKE_SOURCE_DIR}/include 
    /usr/include/yaml-cpp
    ${CMAKE_BINARY_DIR}/Feiteng_autogen/include
)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/Feiteng_autogen/include
  )

# Add new test executable
add_executable(test_face tests/test_face.cpp)
add_dependencies(test_face Feiteng)
force_redefine_file_macro_for_sources(test_face) # __FILE__
target_link_libraries(test_face PRIVATE Feiteng Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::SerialPort ${OpenCV_LIBS} yaml-cpp pthread)

# Add new test executable
add_executable(test_database tests/test_database.cpp)
add_dependencies(test_database Feiteng)
force_redefine_file_macro_for_sources(test_database) # __FILE__
target_link_libraries(test_database PRIVATE Feiteng Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::SerialPort ${OpenCV_LIBS} yaml-cpp pthread)

# Add new test executable
add_executable(test_temperature tests/test_temperature.cpp)
add_dependencies(test_temperature Feiteng)
force_redefine_file_macro_for_sources(test_temperature) # __FILE__
target_link_libraries(test_temperature PRIVATE Feiteng Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::SerialPort ${OpenCV_LIBS} yaml-cpp pthread)

# Add new test executable
add_executable(test_ui tests/test_ui.cpp)
add_dependencies(test_ui Feiteng)
force_redefine_file_macro_for_sources(test_ui) # __FILE__
target_link_libraries(test_ui PRIVATE Feiteng Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::SerialPort ${OpenCV_LIBS} yaml-cpp pthread)
