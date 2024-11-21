cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(Hamming)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Archivos fuente
set(SOURCES
        main.cpp
        hamming.cpp
)

# Incluir encabezados
include_directories(${CMAKE_SOURCE_DIR})

# Crear el ejecutable
add_executable(Hamming ${SOURCES})

# Vincular la biblioteca Winsock para Windows
if (WIN32)
    target_link_libraries(Hamming Ws2_32)
endif()
