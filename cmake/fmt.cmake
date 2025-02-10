# Plugify - Menu System
# Copyright (C) 2025 Wend4r
# Licensed under the MIT license. See LICENSE file in the project root for details.

if(NOT FMT_DIR)
	message(FATAL_ERROR "FMT_DIR is empty")
endif()

set(FMT_BINARY_DIR "fmt::fmt-header-only")

set(FMT_INCLUDE_DIRS
	${FMT_INCLUDE_DIRS}

	${FMT_DIR}/include
)

# add_subdirectory(${FMT_DIR} ${FMT_BINARY_DIR})
