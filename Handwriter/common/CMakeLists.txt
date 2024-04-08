cmake_minimum_required(VERSION 3.14)

project(HandwriterCommon LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

add_library(
    HandwriterCommon STATIC
    dataconversion.h dataconversion.cpp strokerenderbase.h strokerenderbase.cpp
    stroke.h stroke.cpp)

target_link_libraries(HandwriterCommon PRIVATE Qt${QT_VERSION_MAJOR}::Core
                                               Qt${QT_VERSION_MAJOR}::Quick)

target_compile_definitions(HandwriterCommon PRIVATE HANDWRITERCOMMON_LIBRARY)
