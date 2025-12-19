cmake_minimum_required(VERSION 3.16)

project(NotebookApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    Note.cpp
    Note.h
)

add_executable(NotebookApp ${PROJECT_SOURCES})

target_link_libraries(NotebookApp PRIVATE Qt6::Widgets)

# Handle platform-specific settings
if(APPLE)
    set_target_properties(NotebookApp PROPERTIES MACOSX_BUNDLE ON)
endif()
