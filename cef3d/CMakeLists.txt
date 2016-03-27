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
set(PROJECT_NAME "Cef3D")


set(CEF3D_SOURCES
  Cef3DRequired.h
  Cef3D.h
  Cef3D.cc
  Cef3DApp.h
  Cef3DApp.cc
  Cef3DClientHandler.h
  Cef3DClientHandler.cc
  Cef3DV8Handler.h
  Cef3DV8Handler.cc
  Cef3DHelper.h
  Cef3DHelper.cc
  Cef3DBrowserDelegate.h
  Cef3DBrowserDelegate.cc
  Cef3DRendererApp.h
  Cef3DRendererApp.cc
  Cef3DDOM.h
  Cef3DDOM.cc
)

include_directories(${YUME_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${YUME_BOOST_PATH})
include_directories(${CEF_INCLUDE_DIRS})
include_directories(${YUME_3RDPARTY_PATH}/log4cplus/include)


source_group(Cef3D FILES ${CEF3D_SOURCES})

add_definitions(-DBUILDING_CEF3D)

ADD_LIBRARY(${PROJECT_NAME} ${CEF3D_BROWSER_SOURCE} ${CEF3D_RENDERER_SOURCE} ${CEF3D_COMMON_SOURCE} ${CEF3D_SOURCES})

target_link_libraries(${PROJECT_NAME} ${CEF_LIB_DEBUG} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Yume")

set_output_dir(${PROJECT_NAME})
