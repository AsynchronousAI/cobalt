cmake_minimum_required(VERSION 3.1)
project(cobalt LANGUAGES C VERSION 99)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" ON)
option(LUA_BUILD_AS_CXX "Build cobalt as C++" OFF)
option(LUA_ENABLE_SHARED "Build dynamic liblua" ON)
option(FFI "Install the C FFI library for Cobalt" ON)
option(SDL "Install the Cobalt SDL2 bindings library" OFF)
option(SOCKET "Install the socket library for Cobalt" ON)
option(LUA_ENABLE_TESTING "Enable testing for Cobalt" ON)
option(INSTALL "Install Cobalt" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/cmake/")
# Based on the SDL and FFI variable provide them as Flags
if(FFI)
    message("-- Configuring FFI Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_FFI")
endif()
if(SDL)
    message("-- Configuring SDL Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_SDL")
    execute_process(
        COMMAND pkg-config sdl2 --cflags
        OUTPUT_VARIABLE SDL2_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND pkg-config sdl2_image --cflags
        OUTPUT_VARIABLE SDL2_IMAGE_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND pkg-config sdl2_ttf --cflags
        OUTPUT_VARIABLE SDL2_TTF_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND pkg-config sdl2_mixer --cflags
        OUTPUT_VARIABLE SDL2_MIXER_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if (NOT SDL2_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2")
    endif()
    if (NOT SDL2_IMAGE_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2_image")
    endif()
    if (NOT SDL2_TTF_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2_ttf")
    endif()
    if (NOT SDL2_MIXER_CFLAGS)
        message(FATAL_ERROR "Failed to find SDL2_mixer")
    endif()

    set(CMAKE_C_FLAGS "-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer ${SDL2_CFLAGS} ${SDL2_IMAGE_CFLAGS} ${SDL2_TTF_CFLAGS} ${SDL2_MIXER_CFLAGS} ${CMAKE_C_FLAGS}")
endif() 
if (SOCKET)
    message("-- Configuring Socket/cURL Library")
    execute_process(
        COMMAND pkg-config libcurl --libs
        OUTPUT_VARIABLE CURL_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT CURL_LIBS)
        message(FATAL_ERROR "Failed to find libcurl")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CURL_CFLAGS} ${CURL_LIBS} -DCOBALT_SOCKET -lcurl")
endif()
enable_language(CXX)
if(LUA_ENABLE_TESTING)
    enable_testing()
endif()
if (DEBUG)
    message("-- DEBUG: Using these flags for building:\n\t\t${CMAKE_C_FLAGS}")
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
    add_test(NAME bench-series COMMAND cobalt ${TESTARGS} -e "_U=true" bench/series.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-recursive-ack COMMAND cobalt ${TESTARGS} -e "_U=true" bench/recursive-ack.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-fasta COMMAND cobalt ${TESTARGS} -e "_U=true" bench/fasta.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-fannkuch COMMAND cobalt ${TESTARGS} -e "_U=true" bench/fannkuch.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-coroutine-ring COMMAND cobalt ${TESTARGS} -e "_U=true" bench/coroutine-ring.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-life COMMAND cobalt ${TESTARGS} -e "_U=true" bench/life.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-binary-trees COMMAND cobalt ${TESTARGS} -e "_U=true" bench/binary-trees.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve-bit COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nsieve-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nbody COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nbody.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-mandelbrot-bit COMMAND cobalt ${TESTARGS} -e "_U=true" bench/mandelbrot-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-recursive-fib COMMAND cobalt ${TESTARGS} -e "_U=true" bench/recursive-fib.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark_lib COMMAND cobalt ${TESTARGS} -e "_U=true" bench/scimark_lib.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve-bit-fp COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nsieve-bit-fp.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve COMMAND cobalt ${TESTARGS} -e "_U=true" bench/nsieve.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)

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