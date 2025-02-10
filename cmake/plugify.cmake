# Plugify - Menu System
# Copyright (C) 2025 Wend4r
# Licensed under the MIT license. See LICENSE file in the project root for details.

if(NOT PLUGIFY_DIR)
	message(FATAL_ERROR "PLUGIFY_DIR is empty")
endif()

set(PLUGIFY_BINARY_DIR "plugify")

set(PLUGIFY_INCLUDE_DIRS
	${PLUGIFY_INCLUDE_DIRS}

	${PLUGIFY_DIR}/include
)

set(PLUGIFY_COMPILE_DEFINITIONS
	${PLUGIFY_COMPILE_DEFINITIONS}

	# PLUGIFY_STRING_NO_STD_FORMAT=1
)

# add_subdirectory(${PLUGIFY_DIR} ${PLUGIFY_BINARY_DIR})
