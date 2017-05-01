

#
find_package(DirectX)


set(SAMPLE_APP_SRC Main.cpp)
set(COMMON_SRC ../Common/Common.h ../Common/Direct3D12Renderer.h ../Common/Direct3D12Renderer.cpp ../Common/Win32Common.h ../Common/Win32Common.cpp)
add_executable(Cef3D.Osr.Direct3D12 WIN32 ${SAMPLE_APP_SRC} ${COMMON_SRC})

include_directories(${CMAKE_SOURCE_DIR}/Cef3D/Source ${D3D_INCLUDE_DIR} "../Common")
target_link_libraries(Cef3D.Osr.Direct3D12 Cef3D)
target_link_libraries(Cef3D.Osr.Direct3D12 
                        Cef3D
                        libcef_dll_wrapper
                        ${CEF_STANDARD_LIBS}
                        ${PROJECT_SOURCE_DIR}/Cef/Debug/libcef.lib
                        ${D3D_D3D11_LIBRARY}
                        ${D3D_D3DCompiler_LIBRARY}
                        )