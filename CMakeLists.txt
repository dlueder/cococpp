cmake_minimum_required(VERSION 3.16)

project(cococpp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)
find_package(OpenCV REQUIRED)
find_package(nlohmann_json 3.12.0 REQUIRED)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cococpp
        MANUAL_FINALIZATION
        main.cpp
        libs/cocoreader.h libs/cocoreader.cpp
        libs/util.cpp libs/util.h
        ui/preview.h ui/preview.cpp
        ui/preview.ui
    )

else()
    if(ANDROID)
        add_library(cococpp SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(cococpp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(cococpp PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(cococpp PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(cococpp PRIVATE ${OpenCV_LIBS})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.cococpp)
endif()

set_target_properties(cococpp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS cococpp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cococpp)
endif()
