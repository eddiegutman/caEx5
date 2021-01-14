cmake_minimum_required(VERSION 3.17)
project(caEx5 C)

set(CMAKE_C_STANDARD 99)

add_executable(caEx5 src/readBMP.c
        src/readBMP.h
        src/writeBMP.c
        src/writeBMP.h
        src/showBMP.c)

target_link_libraries(caEx5 ${QT_LIBRARIES} m GL GLU glut X11 Xmu Xi)