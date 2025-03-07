cmake_minimum_required(VERSION 3.16)
project(Dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt6_DIR "C:/Qt/6.7.2/mingw_64/lib/cmake/Qt6")
set(Qt6Charts_DIR "C:/Qt/6.7.2/mingw_64/lib/cmake/Qt6Charts")

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
    Charts
)

# Create the library
add_library(dashboardLibrary STATIC
    dashboard.h
    dashboard.cpp
    home.h
    home.cpp
    icons.qrc
)

# Add compile definitions
target_compile_definitions(dashboardLibrary PRIVATE
    QT_CHARTS_LIB
)

# Link the necessary Qt libraries
target_link_libraries(dashboardLibrary PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
)

# Include directories
target_include_directories(dashboardLibrary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
