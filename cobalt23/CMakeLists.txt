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
    "src/lcrypt.c"
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

    # GLMath
    "src/math/quat.c"
    "src/math/rect.c"
    "src/math/enums.c"
    "src/math/.DS_Store"
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

set(LUA_LINKED_LIBRARIES)

if(LUA_BUILD_AS_CXX)
	set_source_files_properties(${LUA_LIB_SRCS} "src/cobalt.c" "src/cobaltc.c" "src/cobaltaot.c" "src/host/minicobalt.c" "pre/main.c" PROPERTIES LANGUAGE CXX )
endif()

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

# CLANG
if (CLANG)
    list(APPEND LUA_LIB_SRCS
        "src/lclang.cpp"
    )

    if (CLANG_PATH_OVERRIDE)
        message("- Overriding Clang Path to: ${CLANG_PATH_OVERRIDE}")
        set(CLANG_DIR "${CLANG_PATH_OVERRIDE}")
    endif()

    find_package(Clang REQUIRED)
    list(APPEND LUA_LINKED_LIBRARIES Clang::Clang)
    include_directories(${CLANG_INCLUDE_DIRS})
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
        set(CURL_DIR "${SDL2_PATH_OVERRIDE}")
    endif()
    if (SDL2_IMAGE_PATH_OVERRIDE)
        message("- Overriding SDL2 Image Path to: ${SDL2_IMAGE_PATH_OVERRIDE}")
        set(CURL_DIR "${SDL2_IMAGE_PATH_OVERRIDE}")
    endif()
    if (SDL2_TTF_PATH_OVERRIDE)
        message("- Overriding SDL2 TTF Path to: ${SDL2_TTF_PATH_OVERRIDE}")
        set(CURL_DIR "${SDL2_TTF_PATH_OVERRIDE}")
    endif()
    if (SDL2_MIXER_PATH_OVERRIDE)
        message("- Overriding SDL2 Mixer Path to: ${SDL2_MIXER_PATH_OVERRIDE}")
        set(CURL_DIR "${SDL2_MIXER_PATH_OVERRIDE}")
    endif()

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    list(APPEND LUA_LINKED_LIBRARIES SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

    set(SDL_LIBRARIES SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer SDL2::SDL2)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
else()
    set(SDL_LIBRARIES)
endif()

# VULKAN
if (VULKAN)
    list(APPEND LUA_LIB_SRCS
        "src/vulkan/getproc.c"
        "src/vulkan/descriptor_set.c"
        "src/vulkan/buffer.c"
        "src/vulkan/display.c"
        "src/vulkan/enums.c"
        "src/vulkan/buffer_view.c"
        "src/vulkan/descriptor_update_template.c"
        "src/vulkan/command_pool.c"
        "src/vulkan/zcheck.c"
        "src/vulkan/pipeline.c"
        "src/vulkan/descriptor_pool.c"
        "src/vulkan/instance.c"
        "src/vulkan/pipeline_layout.c"
        "src/vulkan/layers.c"
        "src/vulkan/display_mode.c"
        "src/vulkan/queue.c"
        "src/vulkan/tracing.c"
        "src/vulkan/validation_cache.c"
        "src/vulkan/debug_utils_messenger.c"
        "src/vulkan/image.c"
        "src/vulkan/device_memory.c"
        "src/vulkan/physical_device.c"
        "src/vulkan/cmd.c"
        "src/vulkan/render_pass.c"
        "src/vulkan/debug_report_callback.c"
        "src/vulkan/query_pool.c"
        "src/vulkan/pipeline_cache.c"
        "src/vulkan/utils.c"
        "src/vulkan/main.c"
        "src/vulkan/semaphore.c"
        "src/vulkan/event.c"
        "src/vulkan/surface.c"

        "src/vulkan/vulkan/vk_format_utils.cpp"
        "src/vulkan/vulkan/vk_layer_config.cpp"
        "src/vulkan/vulkan/vk_layer_extension_utils.cpp"
        "src/vulkan/vulkan/vk_layer_utils.cpp"
        "src/vulkan/vulkan/vk_safe_struct.cpp"
    )
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
    if (CURL_PATH_OVERRIDE)
        message("- Overriding CURL Path to: ${CURL_PATH_OVERRIDE}")
        set(CURL_DIR "${CURL_PATH_OVERRIDE}")
    endif()
    find_package(CURL REQUIRED)
    list(APPEND LUA_LINKED_LIBRARIES CURL::libcurl)
    include_directories(${CURL_INCLUDE_DIRS})
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
    list(APPEND LUA_LINKED_LIBRARIES Python::Python)

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
    #target_link_libraries(lua_static INTERFACE ${PYTHON_LDFLAGS} ${PYTHON_LIBFLAGS})
    #if (LUA_ENABLE_SHARED)
    #    target_link_libraries(lua_shared INTERFACE ${PYTHON_LDFLAGS} ${PYTHON_LIBFLAGS})
    #endif()
endif()

# LLVM
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
    if (LLVM_PATH_OVERRIDE)
        message("- Overriding LLVM Path to: ${LLVM_PATH_OVERRIDE}")
        set(LLVM_DIR "${LLVM_PATH_OVERRIDE}")
    endif()
    find_package(LLVM REQUIRED CONFIG)
    list(APPEND LUA_LINKED_LIBRARIES LLVM::LLVM)
    include_directories(${LLVM_INCLUDE_DIRS})
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
    add_executable(cobaltaot "src/cobaltaot.c")
    
    # Can not use lua_shared because some symbols are not exported
    target_link_libraries(cobaltaot PRIVATE lua_static)
    set_target_properties(cobaltaot PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
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