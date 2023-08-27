cmake_minimum_required(VERSION 3.1)
project(cobalt LANGUAGES C VERSION 99)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" ON)
option(LUA_BUILD_AS_CXX "Build cobalt as C++" OFF)
option(LUA_ENABLE_SHARED "Build dynamic liblua" ON)
option(FFI "Install the C FFI library for Cobalt" ON)
option(SDL "Install the Cobalt SDL2 bindings library" OFF)
option(SOCKET "Install the socket library for Cobalt" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/cmake/")
# Based on the SDL and FFI variable provide them as Flags
if(FFI)
    message("-- Configuring FFI Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DCOBALT_FFI")
endif()
if(SDL)
    message("-- Configuring SDL Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DCOBALT_SDL")

    # Requires SDL, SDL_image, SDL_ttf, and SDL_mixer
    # Find the SDL2 library
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()
if (SOCKET)
    message("-- Configuring Socket Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DCOBALT_SOCKET")
endif()

enable_language(CXX)
if(LUA_ENABLE_TESTING)
    enable_testing()
endif()

if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
    set(TOP_LEVEL TRUE)
else()
    set(TOP_LEVEL FALSE)
endif()

if(TOP_LEVEL)
    option(LUA_BUILD_BINARY "Build cobalt binary" ON)
    option(LUA_BUILD_COMPILER "Build cobaltc compiler" ON)
else()
    option(LUA_BUILD_BINARY "Build cobalt binary" OFF)
    option(LUA_BUILD_COMPILER "Build cobaltc compiler" ON)
endif()

add_subdirectory(cobalt23)
# CLEAR
add_custom_target(cleanlua
    COMMAND make -w clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/lua-cobalt
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Clearing cobalt23"
)
# BUILD
add_custom_target(lua-cobalt
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/lua-cobalt
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building lua-cobalt"
)
add_custom_target(pre
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/pre
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building preprocessor"
)
add_custom_target(mini
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/src/host
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building minicobalt"
)
if(LUA_ENABLE_TESTING)
    add_test(NAME lua-testsuite COMMAND cobalt -e "_U=true" all.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-${CMAKE_PROJECT_VERSION}-tests)
endif()

#if (INSTALL)
add_custom_target(setup
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/install.py
    COMMENT "Installing binaries"
)
add_custom_target(setuppy
    COMMAND py ${CMAKE_CURRENT_SOURCE_DIR}/install.py
    COMMENT "Installing binaries"
)
add_custom_target(setup-py
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/install.py
    COMMENT "Installing binaries"
)
#endif()