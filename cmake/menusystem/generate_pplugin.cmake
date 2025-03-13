# Plugify - Menu System
# Copyright (C) 2025 Wend4r
# Licensed under the MIT license. See LICENSE file in the project root for details.

file(READ "${CMAKE_SOURCE_DIR}/version.txt" VERSION_FILE_CONTENTS)
string(STRIP "${VERSION_FILE_CONTENTS}" VERSION_FILE_CONTENTS)
set(S2MENU_VERSION "${VERSION_FILE_CONTENTS}" CACHE STRING "Set version name")
set(S2MENU_PACKAGE "s2menu" CACHE STRING "Set package name")
string(REPLACE "v" "" S2MENU_VERSION "${S2MENU_VERSION}")

configure_file(
	${CMAKE_SOURCE_DIR}/s2menu.pplugin.in
	${CMAKE_BINARY_DIR}/s2menu.pplugin
)
