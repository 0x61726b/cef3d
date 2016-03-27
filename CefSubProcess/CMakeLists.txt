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
set(PROJECT_NAME "YumeCefSubProcess")

set(SOURCE_FILES Main.cc)

set(HEADER_FILES )

if(OS_LINUX OR OS_MACOSX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOOST_ALL_NO_LIB=1 -g -O0 --coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

include_directories(${YUME_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${YUME_BOOST_PATH})
include_directories(${CEF_INCLUDE_DIRS})
include_directories(${YUME_3RDPARTY_PATH}/log4cplus/include)

add_definitions(-DBOOST_ALL_NO_LIB=1)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES}) #This is to avoid linker error on MSVC so tell that this is a win32 app LOL

target_link_libraries(${PROJECT_NAME} ${CEF_LIB_DEBUG} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
target_link_libraries(${PROJECT_NAME} ${YUME})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Yume")

source_group(${PROJECT_NAME} FILES ${HEADER_FILES} ${SOURCE_FILES})

set_output_dir(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} SDL2main)
