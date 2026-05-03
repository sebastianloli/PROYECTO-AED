cmake_minimum_required(VERSION 3.16)
project(SpreadsheetDS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Rutas Qt6 hardcodeadas para MSYS2 UCRT64
set(Qt6_DIR "/c/msys64/ucrt64/lib/cmake/Qt6")
set(Qt6Widgets_DIR "/c/msys64/ucrt64/lib/cmake/Qt6Widgets")
set(Qt6Core_DIR "/c/msys64/ucrt64/lib/cmake/Qt6Core")
set(Qt6Gui_DIR "/c/msys64/ucrt64/lib/cmake/Qt6Gui")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

message(STATUS "Qt6 version: ${Qt6_VERSION}")

include_directories(include)

# Biblioteca core — SparseMatrix y Operations
add_library(core STATIC
    src/SparseMatrix.cpp
    src/Operations.cpp
)

# Ejecutable principal con GUI Qt6
add_executable(spreadsheet
    src/main.cpp
    src/MainWindow.cpp
    src/GridWidget.cpp
    include/MainWindow.h
    include/GridWidget.h
)
target_link_libraries(spreadsheet core Qt6::Widgets Qt6::Core Qt6::Gui)

# Tests — sin Qt
add_executable(test_matrix tests/test_matrix.cpp)
target_link_libraries(test_matrix core)