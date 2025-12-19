cmake_minimum_required(VERSION 3.16)

project(ImageConverter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

add_executable(ImageConverter ${PROJECT_SOURCES})

target_link_libraries(ImageConverter PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

# Handle platform-specific settings
if(APPLE)
    set_target_properties(ImageConverter PROPERTIES MACOSX_BUNDLE ON)
endif()
