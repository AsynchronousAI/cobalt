# CMAKE 3.1, C99, C++17
cmake_minimum_required(VERSION 3.1)
project(cobalt LANGUAGES C VERSION 99)
project(cobalt LANGUAGES CXX VERSION 17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" ON)
option(LUA_ENABLE_SHARED "Build dynamic liblua" OFF)
option(FFI "Install the C FFI library for Cobalt" ON)
option(SDL "Install the Cobalt SDL2 bindings library" OFF)
option(SOCKET "Install the socket library for Cobalt" ON)
option(LUA_ENABLE_TESTING "Enable testing for Cobalt" ON)
option(INSTALL "Install Cobalt" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_INTERFACE") # -DCOBALT_INTERFACE triggers the `main` symbol to be exported for minicobalt.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/cmake/")
# Based on the SDL and FFI variable provide them as Flags
execute_process(
    COMMAND pkg-config -help
    OUTPUT_VARIABLE PKG_CONFIGEXISTS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (PRE_COMPILED)
set(LUA_USE_DLOPEN ON)
else()
set(LUA_USE_DLOPEN OFF)
endif()
if (NOT PKG_CONFIGEXISTS)
if (NOT IGNORE_PKG_CONFIG)
    message(FATAL_ERROR "pkg-config is not installed on your device and is required for locating dependencies. \n\n\tUse -DIGNORE_PKG_CONFIG=ON to ignore this error (but you will not be able to access SDL, etc.)")
endif()
endif()

if (CROSS)
    message("-- Configuring Crossplatform CPU Build")
else()
    message("-- Configuring Native CPU Build")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
endif()
if (NOOPT)
    message("-- Configuring No Optimization Build")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
else()
    message("-- Configuring Optimized Build")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -flto")
endif()

if(FFI)
    message("-- Configuring FFI Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_FFI")
endif()
if (PYTHON)
    message("-- Adding Python Bindings")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_PYTHON")
    execute_process(
        COMMAND python3-config --cflags
        OUTPUT_VARIABLE PYTHON_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT PYTHON_CFLAGS)
        message(FATAL_ERROR "Failed to find Python3 using python3-config")
    endif()
    execute_process(
        COMMAND python3-config --ldflags
        OUTPUT_VARIABLE PYTHON_LDFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT PYTHON_LDFLAGS)
        message(FATAL_ERROR "Failed to find Python3 using python3-config")
    endif()
    execute_process(
        COMMAND python3-config --libs
        OUTPUT_VARIABLE PYTHON_LIBFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT PYTHON_LIBFLAGS)
        message(FATAL_ERROR "Failed to find Python3 using python3-config")
    endif()
    string(REPLACE "-I" "-L" PYTHON_LINKER_FLAGS "${PYTHON_CFLAGS}")
    if (NOT PYTHON_FLAG)
        message(WARNING "No python library was provided, If errors occur please provide like so:\n\n\t-DPYTHON_FLAG=python3.11")
        set (PYTHON_FLAG "")
    else()
        # concat a -l to the start of the flag
        set (PYTHON_FLAG "-l${PYTHON_FLAG}")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PYTHON_CFLAGS} ${PYTHON_LDFLAGS} ${PYTHON_LIBFLAGS} ${PYTHON_LINKER_FLAGS} ${PYTHON_FLAG}")
endif()
if(SDL)
    message("-- Configuring SDL Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_SDL")
    execute_process(
        COMMAND pkg-config sdl2 --cflags
        OUTPUT_VARIABLE SDL2_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2")
    endif()
    execute_process(
        COMMAND pkg-config SDL2_image --cflags
        OUTPUT_VARIABLE SDL2_IMAGE_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_IMAGE_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2_image")
    endif()
    execute_process(
        COMMAND pkg-config SDL2_ttf --cflags
        OUTPUT_VARIABLE SDL2_TTF_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_TTF_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2_ttf")
    endif()
    execute_process(
        COMMAND pkg-config SDL2_mixer --cflags
        OUTPUT_VARIABLE SDL2_MIXER_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_MIXER_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2_mixer")
    endif()

    # LIBS
    execute_process(
        COMMAND pkg-config sdl2 --libs
        OUTPUT_VARIABLE SDL2_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_LIBS)
        message(FATAL_ERROR "Failed to find SDL2")
    endif()

    execute_process(
        COMMAND pkg-config SDL2_image --libs
        OUTPUT_VARIABLE SDL2_IMAGE_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_IMAGE_LIBS)
        message(FATAL_ERROR "Failed to find SDL2_image")
    endif()

    execute_process(
        COMMAND pkg-config SDL2_ttf --libs
        OUTPUT_VARIABLE SDL2_TTF_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_TTF_LIBS)
        message(FATAL_ERROR "Failed to find SDL2_ttf")
    endif()

    execute_process(
        COMMAND pkg-config SDL2_mixer --libs
        OUTPUT_VARIABLE SDL2_MIXER_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT SDL2_MIXER_LIBS)
        message(FATAL_ERROR "Failed to find SDL2_mixer")
    endif()
    set(CMAKE_C_FLAGS "${SDL2_CFLAGS} ${SDL2_IMAGE_CFLAGS} ${SDL2_TTF_CFLAGS} ${SDL2_MIXER_CFLAGS} ${CMAKE_C_FLAGS} ${SDL2_LIBS} ${SDL2_IMAGE_LIBS} ${SDL2_TTF_LIBS} ${SDL2_MIXER_LIBS}")
endif() 

# Provide platform to language
if (NO_PLAT_LIB)
    message("-- Not using platform libraries")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_UNIX -DNO_WIN")
else()
    message("-- Adding platform libraries")
endif()
if (LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX")
endif()
if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMINGW")
endif()
# Set c++ flags to c flags
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")


enable_language(CXX)
if(LUA_ENABLE_TESTING)
    enable_testing()
endif()
if (DEBUG)
    message("-- DEBUG: Using these flags for building C:\n\t\t${CMAKE_C_FLAGS}")
    message("-- DEBUG: Using these flags for building C++:\n\t\t${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-command-line-argument -Wcomment -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-command-line-argument -Wcomment -g")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()
if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
    set(TOP_LEVEL TRUE)
else()
    set(TOP_LEVEL FALSE)
endif()

if(TOP_LEVEL)
    option(LUA_BUILD_BINARY "Build cobalt binary" ON)
    option(LUA_BUILD_COMPILER "Build cobaltc compiler" ON)
    option(LUA_BUILD_AOT "Build Cobalt to C compiler" ON)
    option(LUA_BUILD_PRE "Build Cobalt preprocessor" ON)
    option(LUA_BUILD_MINI "Build the miniture version of Cobalt" ON)
else()
    option(LUA_BUILD_BINARY "Build cobalt binary" OFF)
    option(LUA_BUILD_COMPILER "Build cobaltc compiler" ON)
    option(LUA_BUILD_AOT "Build Cobalt to C compiler" OFF)
    option(LUA_BUILD_PRE "Build Cobalt preprocessor" ON)
    option(LUA_BUILD_MINI "Build the miniture version of cobalt" OFF)
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
add_custom_target(lua
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/lua-cobalt
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building lua-cobalt"
)
if(POOLTEST)
set(TESTARGS, "-p")
else()
set(TESTARGS, "")
endif()
if(LUA_ENABLE_TESTING)
    add_test(NAME spectralnorm COMMAND cobalt ${TESTARGS} -e "_U=true" spectralnorm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
endif()