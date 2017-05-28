

#
find_package(DirectX)


set(SAMPLE_APP_SRC Main.cpp)
add_executable(OSR_SavetoFile ${SAMPLE_APP_SRC} ${COMMON_SRC})


include_directories(${CMAKE_SOURCE_DIR}/Cef3D/Source ${CMAKE_SOURCE_DIR}/../libpng-libpng16 ${CMAKE_SOURCE_DIR}/../zlib)
target_link_libraries(OSR_SavetoFile Cef3D)
target_link_libraries(OSR_SavetoFile 
                        Cef3D
                        libcef_dll_wrapper
                        ${CEF_STANDARD_LIBS}
                        ${PROJECT_SOURCE_DIR}/Cef/Debug/libcef.lib
                        ${CMAKE_SOURCE_DIR}/Cef3D/Binaries/Win64/libpngd.lib
                        ${CMAKE_SOURCE_DIR}/../zlib/build_x64/Debug/zlibstaticd.lib
                        )