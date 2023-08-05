cmake_minimum_required(VERSION 3.0)

project(lua-lxx)

set(parser_fn "lua-parser")
set(CMAKE_C_FLAGS "-Wall -g")
set(BINARYDIR ${CMAKE_CURRENT_BINARY_DIR}/../..)
add_executable(lua-lxx ${parser_fn}.c)

add_custom_command(
    OUTPUT ${parser_fn}.c
    COMMAND lemon ${CMAKE_CURRENT_SOURCE_DIR}/${parser_fn}.ly
    COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/${parser_fn}.re2c.c >> ${BINARYDIR}/${parser_fn}.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${parser_fn}.ly ${CMAKE_CURRENT_SOURCE_DIR}/lemon ${CMAKE_CURRENT_SOURCE_DIR}/lempar.c ${CMAKE_CURRENT_SOURCE_DIR}/${parser_fn}.re2c.c
)

add_custom_command(
    OUTPUT ${parser_fn}.re2c.c
    COMMAND re2c -Wundefined-control-flow ${CMAKE_CURRENT_SOURCE_DIR}/${parser_fn}.re2c -o ${BINARYDIR}/${parser_fn}.re2c.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${parser_fn}.re2c
)

add_executable(lemon ${CMAKE_CURRENT_SOURCE_DIR}/lemon.c)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove ${BINARYDIR}/lemon ${BINARYDIR}/lua-lxx ${BINARYDIR}/${parser_fn}.out ${BINARYDIR}/${parser_fn}.c ${BINARYDIR}/${parser_fn}.h ${BINARYDIR}/${parser_fn}.re2c.c
)

#add_library(lua-lxx ${parser_fn}.c)
add_library(${parser_fn}.c lemon)
#add_library(${parser_fn}.c ${parser_fn}.re2c.c)
