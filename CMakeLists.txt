# CMAKE 3.1, C99, C++11
cmake_minimum_required(VERSION 3.1)
project(cobalt LANGUAGES C VERSION 99)
project(cobalt LANGUAGES CXX VERSION 11)


option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" ON)
option(LUA_ENABLE_SHARED "Build dynamic liblua" OFF)
option(FFI "Install the C FFI library for Cobalt" ON)
option(SDL "Install the Cobalt SDL2 bindings library" OFF)
option(SOCKET "Install the socket library for Cobalt" ON)
option(LUA_ENABLE_TESTING "Enable testing for Cobalt" ON)
option(INSTALL "Install Cobalt" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_INTERFACE") # -DCOBALT_INTERFACE triggers the `main` symbol to be exported for minicobalt.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/cmake/")
# Based on the SDL and FFI variable provide them as Flags
execute_process(
    COMMAND pkg-config -help
    OUTPUT_VARIABLE PKG_CONFIGEXISTS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT PKG_CONFIGEXISTS)
if (NOT IGNORE_PKG_CONFIG)
    message(FATAL_ERROR "pkg-config is not installed on your device and is required for locating dependencies. \n\n\tUse -DIGNORE_PKG_CONFIG=ON to ignore this error (but you will not be able to access SDL, Clang, etc.)")
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
if (USB)
    message("-- Configuring USB Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_USB")
    execute_process(
        COMMAND pkg-config libusb-1.0 --cflags
        OUTPUT_VARIABLE USB_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT USB_CFLAGS)
        message(FATAL_ERROR "Failed to find libusb-1.0")
    endif()
    execute_process(
        COMMAND pkg-config libusb-1.0 --libs
        OUTPUT_VARIABLE USB_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT USB_LIBS)
        message(FATAL_ERROR "Failed to find libusb-1.0")
    endif()
    
    if (METHOD1)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DINCLUDE_METHOD_1")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DINCLUDE_METHOD_2")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USB_CFLAGS} ${USB_LIBS}")
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
if (CLANG)
    message("-- Configuring Clang Bindings")
    if (LLVM_CONFIG)
        set(LLVM_CONFIG_PATH "${LLVM_CONFIG}")
    else()
        set(LLVM_CONFIG_PATH "llvm-config")
        execute_process(
            COMMAND llvm-config
            OUTPUT_VARIABLE LLVM_CONFIG_EXISTS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if (NOT LLVM_CONFIG_EXISTS)
            message(FATAL_ERROR "llvm-config is not in your system path, please locate it and provide it to cmake like so:\n\t\t-DLLVM_CONFIG=/path/to/llvm-config")
        endif()
    endif()
    execute_process(
        COMMAND ${LLVM_CONFIG_PATH} --includedir --ldflags --libs --cflags
        OUTPUT_VARIABLE CLANG_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT CLANG_LIBS)
        message(FATAL_ERROR "Failed to find libclang, disable cobalt Clang support by doing:\n\t\t-DCLANG=OFF")
    endif()
    string(REPLACE "\n" " " CLANG_LIBS "${CLANG_LIBS}")
    set(CLANG_LIBS "-I${CLANG_LIBS}")
    message("-- ${CLANG_LIBS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_CLANG ${CLANG_LIBS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_LIBS}")
endif()

if (LLVM)
    message("-- Building with JIT and LLVM support")
    if (LLVM_PATH_OVERRIDE)
        set(LLVM_DIR "${LLVM_PATH_OVERRIDE}")
    endif()
    find_package(LLVM REQUIRED CONFIG)
    list(APPEND LUA_LINKED_LIBRARIES LLVM::LLVM)
    include_directories(${LLVM_INCLUDE_DIRS})
    string(REPLACE "-I" "-L" LLVM_LINKER_FLAGS "${LLVM_INCLUDE_DIRS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLLVM=ON ${LLVM_LINKER_FLAGS}")
else()
    message("-- Building without JIT and LLVM support")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

# Provide platform to language
if (LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX")
endif()
if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMINGW")
endif()
# Set c++ flags to c flags
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wc++")


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
    option(LUA_BUILD_LLVM "Build Cobalt LLVM compiler" ON)
    option(LUA_BUILD_PRE "Build Cobalt preprocessor" ON)
    option(LUA_BUILD_MINI "Build the miniture version of Cobalt" ON)
else()
    option(LUA_BUILD_BINARY "Build cobalt binary" OFF)
    option(LUA_BUILD_COMPILER "Build cobaltc compiler" ON)
    option(LUA_BUILD_AOT "Build Cobalt to C compiler" OFF)
    option(LUA_BUILD_LLVM "Build Cobalt LLVM compiler" ON)
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
    add_test(NAME computations COMMAND cobalt ${TESTARGS} -e "_U=true" computations.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-concat COMMAND cobalt ${TESTARGS} -e "_U=true" lib/table/concat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-insert COMMAND cobalt ${TESTARGS} -e "_U=true" lib/table/insert.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-misc COMMAND cobalt ${TESTARGS} -e "_U=true" lib/table/misc.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-pack COMMAND cobalt ${TESTARGS} -e "_U=true" lib/table/pack.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-sort COMMAND cobalt ${TESTARGS} -e "_U=true" lib/table/sort.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-byte COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/byte.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-char COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/char.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-dump COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/dump.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-metatable COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/metatable.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-multiple-functions COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/multiple_functions.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-rep COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/rep.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-reverse COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/reverse.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-sub COMMAND cobalt ${TESTARGS} -e "_U=true" lib/string/sub.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-math-abs COMMAND cobalt ${TESTARGS} -e "_U=true" lib/math/abs.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-math-constants COMMAND cobalt ${TESTARGS} -e "_U=true" lib/math/constants.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-math-random COMMAND cobalt ${TESTARGS} -e "_U=true" lib/math/random.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-assert COMMAND cobalt ${TESTARGS} -e "_U=true" lib/base/assert.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-error COMMAND cobalt ${TESTARGS} -e "_U=true" lib/base/error.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-getsetmetatable COMMAND cobalt ${TESTARGS} -e "_U=true" lib/base/getsetmetatable.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-ipairs COMMAND cobalt ${TESTARGS} -e "_U=true" lib/base/ipairs.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-next COMMAND cobalt ${TESTARGS} -e "_U=true" lib/base/next.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-select COMMAND cobalt ${TESTARGS} -e "_U=true" lib/base/select.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-andor COMMAND cobalt ${TESTARGS} -e "_U=true" lang/andor.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-assignment COMMAND cobalt ${TESTARGS} -e "_U=true" lang/assignment.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-compare_nan COMMAND cobalt ${TESTARGS} -e "_U=true" lang/compare_nan.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-compare COMMAND cobalt ${TESTARGS} -e "_U=true" lang/compare.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-concat COMMAND cobalt ${TESTARGS} -e "_U=true" lang/concat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-for COMMAND cobalt ${TESTARGS} -e "_U=true" lang/for.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-gc COMMAND cobalt ${TESTARGS} -e "_U=true" lang/gc.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-length COMMAND cobalt ${TESTARGS} -e "_U=true" lang/length.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-self COMMAND cobalt ${TESTARGS} -e "_U=true" lang/self.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-table COMMAND cobalt ${TESTARGS} -e "_U=true" lang/table.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-tail_recursion COMMAND cobalt ${TESTARGS} -e "_U=true" lang/tail_recursion.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-constant-number COMMAND cobalt ${TESTARGS} -e "_U=true" lang/constant/number.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-constant-table COMMAND cobalt ${TESTARGS} -e "_U=true" lang/constant/table.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-arith COMMAND cobalt ${TESTARGS} -e "_U=true" lang/meta/arith.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-cat COMMAND cobalt ${TESTARGS} -e "_U=true" lang/meta/cat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-eq COMMAND cobalt ${TESTARGS} -e "_U=true" lang/meta/eq.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-framegap COMMAND cobalt ${TESTARGS} -e "_U=true" lang/meta/framegap.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-newindex COMMAND cobalt ${TESTARGS} -e "_U=true" lang/meta/newindex.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-nomm COMMAND cobalt ${TESTARGS} -e "_U=true" lang/meta/nomm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-upvalue-closure COMMAND cobalt ${TESTARGS} -e "_U=true" lang/upvalue/closure.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-mandelbrot COMMAND cobalt ${TESTARGS} -e "_U=true" bench/mandelbrot.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-spectral-norm COMMAND cobalt ${TESTARGS} -e "_U=true" bench/spectral-norm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-meteor COMMAND cobalt ${TESTARGS} -e "_U=true" bench/meteor.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    #add_test(NAME bench-series COMMAND cobalt ${TESTARGS} -e "_U=true" bench/series.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    #add_test(NAME bench-recursive-ack COMMAND cobalt ${TESTARGS} -e "_U=true" bench/recursive-ack.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-fasta COMMAND cobalt ${TESTARGS} -e "_U=true" bench/fasta.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-fannkuch COMMAND cobalt ${TESTARGS} -e "_U=true" bench/fannkuch.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    #add_test(NAME bench-coroutine-ring COMMAND cobalt ${TESTARGS} -e "_U=true" bench/coroutine-ring.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    #add_test(NAME bench-life COMMAND cobalt ${TESTARGS} -e "_U=true" bench/life.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-binary-trees COMMAND cobalt ${TESTARGS} -e "_U=true" bench/binary-trees.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve-bit COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nsieve-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nbody COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nbody.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-mandelbrot-bit COMMAND cobalt ${TESTARGS} -e "_U=true" bench/mandelbrot-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-recursive-fib COMMAND cobalt ${TESTARGS} -e "_U=true" bench/recursive-fib.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark_lib COMMAND cobalt ${TESTARGS} -e "_U=true" bench/scimark_lib.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve-bit-fp COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nsieve-bit-fp.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nsieve.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)

endif()