cmake_minimum_required(VERSION 3.8)

set(LUA_LIB_SRCS 
    "src/lapi.cpp"
    "src/lcode.cpp"
    "src/lctype.cpp"
    "src/ldebug.cpp"
    "src/ljson.cpp"
    "src/lulib.cpp"
    "src/ldo.cpp"
    "src/ldump.cpp"
    "src/lautoc.c"
    "src/lfunc.cpp"
    "src/lgc.cpp"
    "src/llex.cpp"
    "src/lcrypt.c"
    "src/lmem.cpp"
    "src/lobject.cpp"
    "src/lopcodes.cpp"
    "src/lparser.cpp"
    "src/lstate.cpp"
    "src/lstring.cpp"
    "src/ltable.cpp"
    "src/ltm.cpp"
    "src/lundump.cpp"
    "src/lvm.cpp"
    "src/lzio.cpp"
    "src/lauxlib.cpp"
    "src/lbaselib.cpp"
    "src/lcorolib.c"
    "src/ldblib.c"
    "src/lcore.c" 
    "src/ldevice.c"
    "src/lfile.c"
    "src/lstruct.c" 
    "src/lsignal.c" 
    "src/lchan.c"
    "src/ldyn.c"
    "src/lcolorlib.cpp"
    "src/lbitlib.cpp"
    "src/liolib.cpp"
    "src/lmathlib.c"
    "src/lbitop.c"
    "src/loadlib.cpp"
    "src/loslib.cpp"
    "src/lstrlib.c"
    "src/ltablib.cpp"
    "src/lutf8lib.cpp"
    "src/linit.cpp"
    "src/lplcap.cpp"
    "src/lplcode.c"
    "src/lpltree.cpp"
    "src/fatal.cpp"
    "src/lplvm.cpp"


    # AOT Cobalt compiled
    "src/lpreprocess.c"

    # GLMath
    "src/math/quat.c"
    "src/math/rect.c"
    "src/math/enums.c"
    "src/math/funcs.c"
    "src/math/transform.c"
    "src/math/complex.c"
    "src/math/tracing.c"
    "src/math/box.c"
    "src/math/utils.c"
    "src/math/hostmem.c"
    "src/math/main.c"
    "src/math/viewing.c"
    "src/math/vec.c"
    "src/math/datahandling.c"
    "src/math/mat.c"
    "src/math/num.c"
    "src/math/udata.c"
    "src/math/objects.c"
)


set(TARGETS_TO_INSTALL lua_internal lua_include)

set(LINKED_LIBRARIES)


add_library(lua_internal INTERFACE)

add_library(lua_include INTERFACE)

# FFI
if (FFI)
    list(APPEND LUA_LIB_SRCS
        "src/ffi/call.c"
        "src/ffi/ctype.c"
        "src/ffi/ffi.c"
        "src/ffi/parser.c"
    )
endif()

# SDL
if (SDL)
    list(APPEND LUA_LIB_SRCS
        "src/sdl/log.c"
        "src/sdl/hints.c"
        "src/sdl/palette.c"
        "src/sdl/display.c"
        "src/sdl/enums.c"
        "src/sdl/mouse.c"
        "src/sdl/mixer.c"
        "src/sdl/timer.c"
        "src/sdl/keyboard.c"
        "src/sdl/datastructs.c"
        "src/sdl/gamecontroller.c"
        "src/sdl/texture.c"
        "src/sdl/font.c"
        "src/sdl/tracing.c"
        "src/sdl/animation.c"
        "src/sdl/utils.c"
        "src/sdl/music.c"
        "src/sdl/main.c"
        "src/sdl/messagebox.c"
        "src/sdl/surface.c"
        "src/sdl/chunk.c"
        "src/sdl/pxlfmt.c"
        "src/sdl/sensor.c"
        "src/sdl/renderer.c"
        "src/sdl/flags.c"
        "src/sdl/joystick.c"
        "src/sdl/touch.c"
        "src/sdl/events.c"
        "src/sdl/udata.c"
        "src/sdl/haptic.c"
        "src/sdl/misc.c"
        "src/sdl/cursor.c"
        "src/sdl/glcontext.c"
        "src/sdl/objects.c"
        "src/sdl/window.c"
    )
    if (SDL2_PATH_OVERRIDE)
        message("- Overriding SDL2 Path to: ${SDL2_PATH_OVERRIDE}")
        set(SDL2_DIR "${SDL2_PATH_OVERRIDE}")
    endif()
    if (SDL2_IMAGE_PATH_OVERRIDE)
        message("- Overriding SDL2 Image Path to: ${SDL2_IMAGE_PATH_OVERRIDE}")
        set(SDL2_image_DIR "${SDL2_IMAGE_PATH_OVERRIDE}")
    endif()
    if (SDL2_TTF_PATH_OVERRIDE)
        message("- Overriding SDL2 TTF Path to: ${SDL2_TTF_PATH_OVERRIDE}")
        set(SDL2_ttf_DIR "${SDL2_TTF_PATH_OVERRIDE}")
    endif()
    if (SDL2_MIXER_PATH_OVERRIDE)
        message("- Overriding SDL2 Mixer Path to: ${SDL2_MIXER_PATH_OVERRIDE}")
        set(SDL2_mixer_DIR "${SDL2_MIXER_PATH_OVERRIDE}")
    endif()

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    list(APPEND LINKED_LIBRARIES SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

    set(SDL_LIBRARIES SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer SDL2::SDL2)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
else()
    set(SDL_LIBRARIES)
endif()

# Python 
if (PYTHON)
    list(APPEND LUA_LIB_SRCS
        "src/lpy.c"
        "src/pyl.c"
    )
    message("-- Added python library")

    if (PYTHON_PATH_OVERRIDE)
        message("- Overriding Python Path to: ${PYTHON_PATH_OVERRIDE}")
        set(PYTHON_DIR "${PYTHON_PATH_OVERRIDE}")
    endif()
    find_package(Python REQUIRED)
    list(APPEND LINKED_LIBRARIES Python::Python)

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
    target_link_libraries(lua_internal INTERFACE ${PYTHON_LDFLAGS} ${PYTHON_LIBFLAGS})
    target_link_libraries(lua_include INTERFACE ${PYTHON_LDFLAGS} ${PYTHON_LIBFLAGS})
endif()

add_library(cobalt_static STATIC ${LUA_LIB_SRCS})
target_link_libraries(cobalt_static PRIVATE lua_internal PUBLIC lua_include)
set_target_properties(cobalt_static PROPERTIES
    VERSION "${PACKAGE_VERSION}"
)
list(APPEND TARGETS_TO_INSTALL cobalt_static)
if(NOT BUILD_SHARED_LIBS OR NOT LUA_ENABLE_SHARED)
    add_library(Lua::Library ALIAS cobalt_static)
endif()

if(LUA_ENABLE_SHARED)
    add_library(lua_shared SHARED ${LUA_LIB_SRCS})
    target_link_libraries(lua_shared PRIVATE lua_internal PUBLIC lua_include ${SDL_LIBRARIES})
    set_target_properties(lua_shared PROPERTIES
        VERSION "${PACKAGE_VERSION}"
    )
    if(WIN32)
        target_compile_definitions(lua_shared PRIVATE LUA_BUILD_AS_DLL)
    endif()
    list(APPEND TARGETS_TO_INSTALL lua_shared)
    if(BUILD_SHARED_LIBS)
        add_library(Lua::Library ALIAS lua_shared)
    elseif(NOT TOP_LEVEL)
        set_target_properties(lua_shared PROPERTIES
            EXCLUDE_FROM_ALL ON
        )
    endif()
endif()

target_include_directories(lua_include INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(lua_internal INTERFACE lua_include ${SDL_LIBRARIES})


if(UNIX)
    if(NOT EMSCRIPTEN)
        
        find_library(LIBM m)
        if(NOT LIBM)
            message(FATAL_ERROR "libm not found and is required by lua")
        endif()
        target_compile_definitions(lua_internal INTERFACE "LUA_USE_POSIX")
        target_link_libraries(lua_internal INTERFACE m)
        list(APPEND LINKED_LIBRARIES m)
        if(LUA_SUPPORT_DL)
            target_compile_definitions(lua_internal INTERFACE "LUA_USE_DLOPEN")
            target_link_libraries(lua_internal INTERFACE "${CMAKE_DL_LIBS}")
            list(APPEND LINKED_LIBRARIES "${CMAKE_DL_LIBS}")
        endif()
    endif()

	target_compile_options(lua_internal
		INTERFACE "-Wall" "-Wextra"
	)
elseif(Win32)
    target_compile_options(lua_internal
		INTERFACE "/Wall"
	)
endif()

if(LUA_BUILD_BINARY)
    include(CheckIncludeFile)
    CHECK_INCLUDE_FILE("readline/readline.h" HAVE_READLINE_READLINE_H)

    add_executable(cobalt "src/cobalt.c")

    # Can not use lua_shared because some symbols are not exported
    target_link_libraries(cobalt PRIVATE cobalt_static)
    set_target_properties(cobalt PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    if (HAVE_READLINE_READLINE_H)
        target_compile_definitions(cobalt PRIVATE "LUA_USE_READLINE")
        target_link_libraries(cobalt PUBLIC readline)
    endif()
    list(APPEND TARGETS_TO_INSTALL cobalt)
endif()

if(LUA_BUILD_COMPILER)
    add_executable(cobaltc "src/cobaltc.cpp")
    target_link_libraries(cobaltc PRIVATE cobalt_static)
    set_target_properties(cobaltc PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltc)
endif()

if(LUA_BUILD_AOT)
    add_executable(cobaltaot "src/cobaltaot.c")
    
    # Can not use lua_shared because some symbols are not exported
    target_link_libraries(cobaltaot PRIVATE cobalt_static)
    set_target_properties(cobaltaot PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltaot)
endif()


if(LUA_BUILD_PRE)
    add_executable(cobaltpre "pre/main.c")
    target_link_libraries(cobaltpre PRIVATE cobalt_static)
    set_target_properties(cobaltpre PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltpre)
endif()

if(LUA_BUILD_MINI)
    add_executable(cobaltmini "src/host/minicobalt.c")
    target_link_libraries(cobaltmini PRIVATE cobalt_static)
    set_target_properties(cobaltmini PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltmini)
endif()

install(TARGETS ${TARGETS_TO_INSTALL}
        EXPORT CobaltTargets
)

install(DIRECTORY src/ TYPE INCLUDE)

include(CMakePackageConfigHelpers)

get_target_property(LUA_EXPORT_LIBRARY Lua::Library ALIASED_TARGET)
write_basic_package_version_file(
    Config.cmake
    VERSION 23
    COMPATIBILITY SameMajorVersion
)

install(EXPORT CobaltTargets
        FILE CobaltTargets.cmake
        DESTINATION "share/cmake/cobalt"
        NAMESPACE Lua::
)

configure_package_config_file(
    Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake"
    INSTALL_DESTINATION "share/cmake/cobalt"
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake"
        DESTINATION "share/cmake/cobalt"
)