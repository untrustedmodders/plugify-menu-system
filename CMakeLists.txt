# Plugify - Menu System
# Copyright (C) 2025 Wend4r
# Licensed under the MIT license. See LICENSE file in the project root for details.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(POLICY CMP0092)
	cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()

project(plugify-menu-system
	VERSION 1.0.0
	DESCRIPTION "Plugify - Menu System"
	HOMEPAGE_URL "https://github.com/Wend4r/mms2-sample"
	LANGUAGES C CXX
)

string(REGEX REPLACE "^[^-_]*[-_]([^-_]*)[-_]*(.*)" "\\1\\2" PROJECT_NAME_SUBSTRING "${PROJECT_NAME}")
string(TOLOWER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_LOWER)
string(TOUPPER "${PROJECT_NAME_SUBSTRING}" PROJECT_NAME_UPPER)
set(PROJECT_PACKAGE "${PROJECT_NAME}-${PROJECT_VERSION}")

set(PROJECT_OUTPUT_NAME "s2menu")

set(PROJECT_AUTHOR "Wend4r")
set(PROJECT_DESCRIPTION_FULL "Plugify - The Menu System for Source 2 games")
set(PROJECT_LICENSE "MIT")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP PROJECT_BUILD_TIME "%H:%M:%S")

set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SYMBOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sym")
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
set_or_external_dir(LOGGER_DIR "logger")
set_or_external_dir(SOURCESDK_DIR "sourcesdk")

include(cmake/platform/shared.cmake)

include(cmake/dynlibutils.cmake)
include(cmake/fmt.cmake)
include(cmake/logger.cmake)
include(cmake/sourcesdk.cmake)

set(SOURCE_FILES
	${SOURCE_EXPORT_DIR}/menu.cpp
	${SOURCE_DIR}/menusystem_plugin.cpp
)

if(LINUX)
	set(LINK_OPTIONS
		${LINK_OPTIONS}

		-Wl,--version-script,${SYMBOLS_DIR}/version_script.lds
	)
endif()

set(COMPILE_DEFINITIONS
	PLUGIFY_IS_DEBUG=$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>
	PLUGIFY_IS_RELEASE=$<STREQUAL:${CMAKE_BUILD_TYPE},Release>

	PLUGIFY_LIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
	PLUGIFY_LIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
)

set(INCLUDE_DIRS
	${INCLUDE_DIR}
	${EXPORTS_DIR}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

include(cmake/menusystem/generate_exports.cmake)
include(cmake/menusystem/generate_pplugin.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES
	C_STANDARD 20
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF

	CXX_STANDARD 20
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

target_compile_definitions(${PROJECT_NAME} PRIVATE ${COMPILE_DEFINITIONS} ${SOURCESDK_COMPILE_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS} ${DYNLIBUTILS_INCLUDE_DIRS} ${FMT_INCLUDE_DIRS} ${LOGGER_INCLUDE_DIRS} ${SOURCESDK_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES} ${DYNLIBUTILS_BINARY_DIR} ${LOGGER_BINARY_DIR} ${SOURCESDK_TIER0_BINARY_DIR} ${SOURCESDK_TIER1_BINARY_DIR})
