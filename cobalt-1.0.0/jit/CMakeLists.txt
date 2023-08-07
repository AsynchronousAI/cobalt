cmake_minimum_required(VERSION 3.0)
project(luajit)

set(MAJVER 2)
set(MINVER 1)
set(RELVER 0)
set(PREREL "-beta3")
set(VERSION "${MAJVER}.${MINVER}.${RELVER}${PREREL}")
set(ABIVER 5.1)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

add_subdirectory(src)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/jit" DESTINATION "share/luajit-${VERSION}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/lua" DESTINATION "share")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src" DESTINATION "include/luajit-${MAJVER}.${MINVER}" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/etc" DESTINATION "share/luajit-${VERSION}" FILES_MATCHING PATTERN "*.1")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cobaltjit.pc"
"prefix=${CMAKE_INSTALL_PREFIX}
multilib=lib

Name: cobaltjit
Description: Just-In-Time Compiler for Lua
Version: ${VERSION}
Libs: -L\${prefix}/\${multilib} -lluajit-${ABIVER}
Cflags: -I\${prefix}/include/luajit-${MAJVER}.${MINVER}
")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cobaltjit.pc" DESTINATION "lib/pkgconfig")
