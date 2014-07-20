cmake_minimum_required(VERSION 2.8.12)

project(ObsQtText)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(LibObs REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
#find_package(Qt5WebKitWidgets REQUIRED)

set(ObsWebkit_SOURCES
	src/main.cpp
	src/qttext.cpp)

set(ObsWebkit_HEADERS
	src/qttext.h)

add_library(ObsQtText MODULE
	${ObsWebkit_SOURCES}
	${ObsWebkit_HEADERS})
target_link_libraries(ObsQtText
	libobs
	Qt5::Gui
	Qt5::Widgets
	Qt5::OpenGL
#	Qt5::WebKitWidgets
)

get_target_property(INCDIRS libobs INTERFACE_INCLUDE_DIRECTORIES)

message(STATUS "${INCDIRS}")

install(TARGETS ObsQtText
	LIBRARY DESTINATION "${LIBOBS_PLUGIN_DESTINATION}"
	RUNTIME DESTINATION "${LIBOBS_PLUGIN_DESTINATION}")
