set(SAMPLE_APP_SRC Main.cpp)

add_executable(SampleApp WIN32 ${SAMPLE_APP_SRC})

include_directories(${CMAKE_SOURCE_DIR}/Cef3D)
target_link_libraries(SampleApp Cef3D)