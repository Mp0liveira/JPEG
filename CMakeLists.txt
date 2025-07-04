cmake_minimum_required(VERSION 3.10)
project(JPEGCompression)

set(CMAKE_CXX_STANDARD 14)

# O find_package vai encontrar o OpenCV instalado pelo apt-get sem nenhum esforço.
find_package(OpenCV REQUIRED)

# O include_directories e o target_link_libraries continuam iguais.
include_directories(${OpenCV_INCLUDE_DIRS} includes)

add_executable(JPEG main.cpp src/JPEG.cpp src/ac_bitstream.cpp)

target_link_libraries(JPEG ${OpenCV_LIBS})

message(STATUS "OpenCV encontrado: ${OpenCV_VERSION}")