cmake_minimum_required(VERSION 3.16)

project(Dashboard VERSION 0.1 LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets  REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets  REQUIRED)

add_library(dashboardLibrary STATIC
dashboard.h dashboard.cpp
home.h home.cpp)

# Link the necessary Qt libraries to dashboardLibrary
target_link_libraries(dashboardLibrary PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Include directories for the Qt library (optional if CMake automatically finds them)
target_include_directories(dashboardLibrary PRIVATE ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})
