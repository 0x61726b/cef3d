

#
find_package(DirectX)


set(SAMPLE_APP_SRC Main.cpp)
add_executable(MMD_Profile ${SAMPLE_APP_SRC} ${COMMON_SRC})


include_directories(${CMAKE_SOURCE_DIR}/Cef3D/Source ${CMAKE_SOURCE_DIR}/../libpng-libpng16 ${CMAKE_SOURCE_DIR}/../zlib)
target_link_libraries(MMD_Profile Cef3D)
target_link_libraries(MMD_Profile 
                        Cef3D
                        libcef_dll_wrapper
                        ${CEF_STANDARD_LIBS}
                        ${PROJECT_SOURCE_DIR}/Cef/Debug/libcef.lib
                        ${CMAKE_SOURCE_DIR}/../libpng-libpng16/build/Debug/libpng16_staticd.lib
                        ${CMAKE_SOURCE_DIR}/../zlib/build/Debug/zlibstaticd.lib
                        )