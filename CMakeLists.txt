# Plugify - Menu System
# Copyright (C) 2025 Wend4r
# Licensed under the MIT license. See LICENSE file in the project root for details.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(POLICY CMP0092)
	cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()

project(plugify-menu_system
	VERSION 1.0.0
	DESCRIPTION "Plugify - Menu System"
	HOMEPAGE_URL "https://github.com/Wend4r/mms2-sample"
	LANGUAGES C CXX
)

string(REGEX REPLACE "^[^-_]*[-_]([^-_]*)[-_]*(.*)" "\\1\\2" PROJECT_NAME_SUBSTRING "${PROJECT_NAME}")
string(TOLOWER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_LOWER)
string(TOUPPER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_UPPER)

set(PROJECT_OUTPUT_NAME "menu")

set(PROJECT_AUTHOR "Wend4r")
set(PROJECT_DESCRIPTION_FULL "Plugify - The Menu System for Source 2 games")
set(PROJECT_LICENSE "MIT")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP PROJECT_BUILD_TIME "%H:%M:%S")

set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCE_EXPORT_DIR "${SOURCE_DIR}/export")
set(EXPORTS_DIR "${CMAKE_BINARY_DIR}/exports")

function(set_or_external_dir VAR_NAME DEFAULT_DIR)
	if(${VAR_NAME})
		file(TO_CMAKE_PATH "${${VAR_NAME}}" ${VAR_NAME})
	else()
		set(${VAR_NAME} "${EXTERNAL_DIR}/${DEFAULT_DIR}")
	endif()

	set(${VAR_NAME} "${${VAR_NAME}}" PARENT_SCOPE)
endfunction()

set_or_external_dir(DYNLIBUTILS_DIR "dynlibutils")
set_or_external_dir(FMT_DIR "fmt")
set_or_external_dir(PLUGIFY_DIR "plugify")
set_or_external_dir(SOURCESDK_DIR "sourcesdk")

include(cmake/platform/shared.cmake)

include(cmake/dynlibutils.cmake)
include(cmake/fmt.cmake)
include(cmake/plugify.cmake)
include(cmake/sourcesdk.cmake)

set(SOURCE_FILES
	${SOURCE_EXPORT_DIR}/menu.cpp
	${SOURCE_DIR}/plugin.cpp
)

set(INCLUDE_DIRS
	${INCLUDE_DIR}
	${EXPORTS_DIR}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

include(cmake/menusystem/generate_exports.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES
	C_STANDARD 17
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF

	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_OUTPUT_NAME})

if(WINDOWS)
	set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MACOS)
	set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES "x86_64")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILER_OPTIONS} ${SOURCESDK_COMPILE_OPTIONS})
target_link_options(${PROJECT_NAME} PRIVATE ${LINK_OPTIONS} ${SOURCESDK_LINK_OPTIONS})

target_compile_definitions(${PROJECT_NAME} PRIVATE ${COMPILE_DEFINITIONS} ${PLUGIFY_COMPILE_DEFINITIONS} ${SOURCESDK_COMPILE_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS} ${DYNLIBUTILS_INCLUDE_DIRS} ${FMT_INCLUDE_DIRS} ${PLUGIFY_INCLUDE_DIRS} ${SOURCESDK_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES} ${DYNLIBUTILS_BINARY_DIR} ${SOURCESDK_BINARY_DIR})
