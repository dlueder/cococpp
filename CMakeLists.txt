cmake_minimum_required(VERSION 3.16)

project(cococpp LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)
find_package(OpenCV REQUIRED)
find_package(nlohmann_json 3.12.0 REQUIRED)

add_executable(cococpp
  main.cpp
  libs/cocoreader.h libs/cocoreader.cpp
  libs/util.cpp libs/util.h
  ui/preview.h ui/preview.cpp
  ui/preview.ui
)
target_link_libraries(cococpp Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(cococpp nlohmann_json::nlohmann_json)
target_link_libraries(cococpp ${OpenCV_LIBS})

include(GNUInstallDirs)
install(TARGETS cococpp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
