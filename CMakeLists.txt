cmake_minimum_required(VERSION 3.15)

project(bed LANGUAGES C VERSION 0.1)

set(CMAKE_C_COMPILER clang)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT")

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

add_compile_options(-gdwarf-4 -ggdb -Wall -Wextra)


file(GLOB SRC_FILES ${SRC_DIR}/*/*.c ${SRC_DIR}/*.c)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(GLOB BIN_DIR ${CMAKE_BINARY_DIR}/bin)
configure_file("FiraMono.ttf" "FiraMono.ttf" COPYONLY)

add_executable(${PROJECT_NAME} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_custom_target(run 
	COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}
	COMMENT "${PROJECT_NAME} : ")

add_link_options(-lm)

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_ttf)

#set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)
#include(${TEST_DIR}/CTest.cmake )
