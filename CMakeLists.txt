cmake_minimum_required(VERSION 3.16)
project(git-manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Network
)
qt_standard_project_setup()

file(GLOB SRC src/*.cpp)
file(GLOB HEAD include/*.h)
file(GLOB UI include/*.ui)
set(PROJECT_SOURCES
    ${SRC}
    ${HEAD}
    ${UI}
)

qt_add_executable(git-manager ${PROJECT_SOURCES})
set_target_properties(
    git-manager
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
               LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

set_target_properties(git-manager
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(git-manager
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)

target_include_directories(git-manager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)