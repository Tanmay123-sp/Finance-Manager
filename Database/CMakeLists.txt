cmake_minimum_required(VERSION 3.16)

project(Database VERSION 0.1 LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql REQUIRED)

add_library(databaseLibrary STATIC
database.h database.cpp)

# Link the necessary Qt libraries to databaseLibrary
target_link_libraries(databaseLibrary PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Include directories for the Qt library (optional if CMake automatically finds them)
target_include_directories(databaseLibrary PRIVATE ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})
