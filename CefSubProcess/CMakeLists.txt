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


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${CEF_LIB_DEBUG} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
target_link_libraries(${PROJECT_NAME} ${YUME})

# set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Yume")

source_group(${PROJECT_NAME} FILES ${SOURCE_FILES})

set_output_dir(${PROJECT_NAME})
