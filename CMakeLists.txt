cmake_minimum_required(VERSION 3.5)

project(Feiteng LANGUAGES CXX)

include (cmake/utils.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Find Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)

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

# If using Android
if(ANDROID)
  add_library(Feiteng SHARED
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UIS}
  )
else()
  add_executable(Feiteng
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UIS}
  )
endif()

force_redefine_file_macro_for_sources(Feiteng) # __FILE__

# Link libraries
target_link_libraries(Feiteng PRIVATE Qt5::Widgets ${OpenCV_LIBS} yaml-cpp pthread)  # Link OpenCV libraries here)

# Include directories
target_include_directories(Feiteng PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/include/yaml-cpp)


# If you have tests
# enable_testing()
# add_subdirectory(tests)
