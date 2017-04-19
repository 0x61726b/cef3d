################################################################################
#Yume Engine MIT License (MIT)

# Copyright (c) 2015 arkenthera
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# File : CMakeLists.txt
# Date : 8.27.2015
# Comments :
################################################################################
set(PROJECT_NAME "CefSubProcess")

set(SOURCE_FILES Main.cc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CEF_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/Cef3D/Source)


set(BINARY_PREFIX "")

if(OS_WINDOWS)
    set(BINARY_PREFIX "Win")
endif()

if(OS_LINUX)
	set(BINARY_PREFIX "Linux")
endif()

if(PROJECT_ARCH_X64 EQUAL 1)
    set(BINARY_PREFIX "${BINARY_PREFIX}64")
endif()

if(PROJECT_ARCH_X64 EQUAL 0)
    set(BINARY_PREFIX "${BINARY_PREFIX}86")
endif()

set(BINARY_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/Cef3D/Binaries/${BINARY_PREFIX})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${CEF_LIB_DEBUG} libcef_dll_wrapper Cef3D ${CEF_STANDARD_LIBS} ${PROJECT_SOURCE_DIR}/Cef/Debug/libcef.lib)


