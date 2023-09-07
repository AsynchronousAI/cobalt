cmake_minimum_required(VERSION 3.8)

set(LUA_LIB_SRCS 
    "src/lapi.c"
    "src/lcode.c"
    "src/lctype.c"
    "src/ldebug.c"
    "src/ljson.c"
    "src/lfpconv.c"
    "src/lstrbuf.c"
    "src/lulib.c"
    "src/lregex.c"
    "src/ldo.c"
    "src/ldump.c"
    "src/lautoc.c"
    "src/lfunc.c"
    "src/lgc.c"
    "src/llex.c"
    "src/lmem.c"
    "src/lobject.c"
    "src/lopcodes.c"
    "src/lparser.c"
    "src/lstate.c"
    "src/lstring.c"
    "src/ltable.c"
    "src/ltm.c"
    "src/lundump.c"
    "src/lvm.c"
    "src/lzio.c"
    "src/lauxlib.c"
    "src/lbaselib.c"
    "src/lcorolib.c"
    "src/ldblib.c"
    "src/l2Dlib.c"
    "src/l3Dlib.c"
	"src/l3transform.c"
    "src/lcore.c" 
    "src/ldevice.c"
    "src/lfile.c"
    "src/lstruct.c" 
    "src/lsignal.c" 
    "src/lchan.c"
    "src/lcolorlib.c"
    "src/lbitlib.c"
    "src/liolib.c"
    "src/lmathlib.c"
    "src/lbitop.c"
    "src/loadlib.c"
    "src/loslib.c"
    "src/lstrlib.c"
    "src/ltablib.c"
    "src/lutf8lib.c"
    "src/linit.c"
    "src/lplcap.c"
    "src/lplcode.c"
    "src/lplprint.c"
    "src/lpltree.c"
    "src/lplvm.c"


    # AOT Cobalt compiled
    "src/lpreprocess.c"
)


# FFI
if (FFI)
    list(APPEND LUA_LIB_SRCS
        "src/ffi/call.c"
        "src/ffi/ctype.c"
        "src/ffi/ffi.c"
        "src/ffi/parser.c"
    )
endif()

# CLANG
if (CLANG)
    list(APPEND LUA_LIB_SRCS
        "src/lclang.cpp"
    )

    find_package(Clang REQUIRED)
    list(APPEND LUA_LINKED_LIBRARIES Clang::Clang)

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
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    list(APPEND LUA_LINKED_LIBRARIES SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

    set(SDL_LIBRARIES SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer SDL2::SDL2)
else()
    set(SDL_LIBRARIES)
endif()

# SOCKET
if (SOCKET)
    list(APPEND LUA_LIB_SRCS
        "src/curl/lcurlapi.c"
        "src/curl/lcurl.c"
        "src/curl/lcutils.c"
        "src/curl/lcerror.c"
        "src/curl/lcmime.c"
        "src/curl/lchttppost.c"
        "src/curl/lcmulti.c"
        "src/curl/lceasy.c"
        "src/curl/l52util.c"
        "src/curl/lcshare.c"
    )
    find_package(CURL REQUIRED)
    list(APPEND LUA_LINKED_LIBRARIES CURL::libcurl)
endif()

# LLVM
set(LLVM OFF)
if (LLVM)
    list(APPEND LUA_LIB_SRCS
        "src/lllarith.cpp"
        "src/lllcompiler.cpp"
        "src/lllcompilerstate.cpp"
        "src/lllengine.cpp"
        "src/llllib.c"
        "src/llllogical.cpp"
        "src/lllopcode.cpp"
        "src/lllruntime.cpp"
        "src/llltableset.cpp"
        "src/lllvalue.cpp"
        "src/lllvararg.cpp"
        "src/lllcore.cpp"
    )
    find_package(LLVM REQUIRED CONFIG)
    list(APPEND LUA_LINKED_LIBRARIES LLVM::LLVM)
endif()

set(TARGETS_TO_INSTALL lua_internal lua_include)

set(LUA_LINKED_LIBRARIES)

if(LUA_BUILD_AS_CXX)
	set_source_files_properties(${LUA_LIB_SRCS} "src/cobalt.c" "src/cobaltc.c" "src/cobaltaot.c" "src/host/minicobalt.c" "pre/main.c" PROPERTIES LANGUAGE CXX )
endif()

add_library(lua_internal INTERFACE)

add_library(lua_include INTERFACE)

target_include_directories(lua_include INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(lua_internal INTERFACE lua_include ${SDL_LIBRARIES})

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

add_library(lua_static STATIC ${LUA_LIB_SRCS})
target_link_libraries(lua_static PRIVATE lua_internal PUBLIC lua_include)
set_target_properties(lua_static PROPERTIES
    VERSION "${PACKAGE_VERSION}"
)
list(APPEND TARGETS_TO_INSTALL lua_static)
if(NOT BUILD_SHARED_LIBS OR NOT LUA_ENABLE_SHARED)
    add_library(Lua::Library ALIAS lua_static)
endif()
if(UNIX)
    if(NOT EMSCRIPTEN)
        
        find_library(LIBM m)
        if(NOT LIBM)
            message(FATAL_ERROR "libm not found and is required by lua")
        endif()
        target_compile_definitions(lua_internal INTERFACE "LUA_USE_POSIX")
        target_link_libraries(lua_internal INTERFACE m)
        list(APPEND LUA_LINKED_LIBRARIES m)
        if(LUA_SUPPORT_DL)
            target_compile_definitions(lua_internal INTERFACE "LUA_USE_DLOPEN")
            target_link_libraries(lua_internal INTERFACE "${CMAKE_DL_LIBS}")
            list(APPEND LUA_LINKED_LIBRARIES "${CMAKE_DL_LIBS}")
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
    target_link_libraries(cobalt PRIVATE lua_static)
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
    add_executable(cobaltc "src/cobaltc.c")
    target_link_libraries(cobaltc PRIVATE lua_static)
    set_target_properties(cobaltc PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltc)
endif()

if(LUA_BUILD_AOT)
    include(CheckIncludeFile)
    CHECK_INCLUDE_FILE("readline/readline.h" HAVE_READLINE_READLINE_H)

    add_executable(cobaltaot "src/cobaltaot.c")
    # Can not use lua_shared because some symbols are not exported
    target_link_libraries(cobaltaot PRIVATE lua_static)
    set_target_properties(cobaltaot PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    if (HAVE_READLINE_READLINE_H)
        target_compile_definitions(cobaltaot PRIVATE "LUA_USE_READLINE")
        target_link_libraries(cobaltaot PUBLIC readline)
    endif()
    list(APPEND TARGETS_TO_INSTALL cobaltaot)
endif()

if(LUA_BUILD_PRE)
    add_executable(cobaltpre "pre/main.c")
    target_link_libraries(cobaltpre PRIVATE lua_static)
    set_target_properties(cobaltpre PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltpre)
endif()

if(LUA_BUILD_MINI)
    add_executable(cobaltmini "src/host/minicobalt.c")
    target_link_libraries(cobaltmini PRIVATE lua_static)
    set_target_properties(cobaltmini PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    list(APPEND TARGETS_TO_INSTALL cobaltmini)
endif()

install(TARGETS ${TARGETS_TO_INSTALL}
        EXPORT LuaTargets
)

install(DIRECTORY src/ TYPE INCLUDE)

include(CMakePackageConfigHelpers)

get_target_property(LUA_EXPORT_LIBRARY Lua::Library ALIASED_TARGET)
write_basic_package_version_file(
    Config.cmake
    VERSION 23
    COMPATIBILITY SameMajorVersion
)

install(EXPORT LuaTargets
        FILE LuaTargets.cmake
        DESTINATION "share/cmake/Lua"
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