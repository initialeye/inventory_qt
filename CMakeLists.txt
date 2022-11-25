cmake_minimum_required(VERSION 3.5)

project(test_inventory VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VERSION_MAJOR 5)
set(QT_VERSION_MINOR 15)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia Sql)

FILE(GLOB SOURCES src/*.cpp)
FILE(GLOB HEADERS include/*.hpp)
FILE(GLOB RESOURCES res/*.qrc)

qt5_add_resources (RCC_SOURCES ${RESOURCES})
set(PROJECT_SOURCES ${SOURCES} ${HEADERS} ${RCC_SOURCES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test_inventory
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
	)
else()
    if(ANDROID)
        add_library(test_inventory SHARED
            ${PROJECT_SOURCES}
		)
    else()
        add_executable(test_inventory
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(test_inventory PRIVATE include)

target_link_libraries(test_inventory PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia
	Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties(test_inventory PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS test_inventory
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test_inventory)
endif()
