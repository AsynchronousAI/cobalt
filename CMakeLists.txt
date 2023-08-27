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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_FFI")
endif()
if(SDL)
    message("-- Configuring SDL Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_SDL")

    # Requires SDL, SDL_image, SDL_ttf, and SDL_mixer
    # Find the SDL2 library
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()
if (SOCKET)
    message("-- Configuring Socket Library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCOBALT_SOCKET")
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
    add_test(NAME spectralnorm COMMAND cobalt -e "_U=true" spectralnorm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME computations COMMAND cobalt -e "_U=true" computations.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)

    # ALL TESTS:
    # lib
    ## bit.cobalt
    ## table
    ### concat.cobalt
    ### insert.cobalt
    ### misc.cobalt
    ### new .cobalt
    ### pack.cobalt
    ### remove.cobalt
    ### sort.cobalt
    ## string
    ### format
    #### num.cobalt
    ### byte.cobalt
    ### char.cobalt
    ### dump.cobalt
    ### len.cobalt
    ### lower_upper.cobalt
    ### metatable.cobalt
    ### multiple_functions.cobalt
    ### rep.cobalt
    ### reverse.cobalt
    ### sub.cobalt
    ## math
    ### abs.cobalt
    ### constants.cobalt
    ### random.cobalt
    ## base
    ### assert.cobalt
    ### error.cobalt
    ### getfenv.cobalt
    ### getsetmetatable.cobalt
    ### ipairs.cobalt
    ### next.cobalt
    ### pairs.cobalt
    ### select.cobalt
    ### tonumber_tostring.cobalt
 
    add_test(NAME lib-table-concat COMMAND cobalt -e "_U=true" lib/table/concat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-insert COMMAND cobalt -e "_U=true" lib/table/insert.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-misc COMMAND cobalt -e "_U=true" lib/table/misc.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-pack COMMAND cobalt -e "_U=true" lib/table/pack.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-sort COMMAND cobalt -e "_U=true" lib/table/sort.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-byte COMMAND cobalt -e "_U=true" lib/string/byte.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-char COMMAND cobalt -e "_U=true" lib/string/char.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-dump COMMAND cobalt -e "_U=true" lib/string/dump.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-metatable COMMAND cobalt -e "_U=true" lib/string/metatable.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-multiple-functions COMMAND cobalt -e "_U=true" lib/string/multiple_functions.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-rep COMMAND cobalt -e "_U=true" lib/string/rep.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-reverse COMMAND cobalt -e "_U=true" lib/string/reverse.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-sub COMMAND cobalt -e "_U=true" lib/string/sub.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-math-abs COMMAND cobalt -e "_U=true" lib/math/abs.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-math-constants COMMAND cobalt -e "_U=true" lib/math/constants.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-math-random COMMAND cobalt -e "_U=true" lib/math/random.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-assert COMMAND cobalt -e "_U=true" lib/base/assert.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-error COMMAND cobalt -e "_U=true" lib/base/error.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-getsetmetatable COMMAND cobalt -e "_U=true" lib/base/getsetmetatable.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-ipairs COMMAND cobalt -e "_U=true" lib/base/ipairs.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-next COMMAND cobalt -e "_U=true" lib/base/next.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-select COMMAND cobalt -e "_U=true" lib/base/select.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    
    # lang
    # andor.cobalt
    # assignment.cobalt
    # compare_nan.cobalt
    # compare.cobalt
    # concat.cobalt
    # coroutine.cobalt
    # for.cobalt
    # gc.cobalt
    # goto.cobalt
    # length.vobalt
    # modulo.cobalt
    # self.cobalt
    # table.cobalt
    # tail_recursion.cobalt
    # constant
    ## number.cobalt
    ## table.cobalt
    # meta
    ## arith.cobalt
    ## call.cobalt
    ## cat.cobalt
    ## comp.cobalt
    ## debuginfo.cobalt
    ## eq.cobalt
    ## framegap.cobalt
    ## index.cobalt
    ## len.cobalt
    ## newindex.cobalt
    ## nomm.cobalt
    # upvalue
    ## closure.cobalt

    add_test(NAME lang-andor COMMAND cobalt -e "_U=true" lang/andor.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-assignment COMMAND cobalt -e "_U=true" lang/assignment.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-compare_nan COMMAND cobalt -e "_U=true" lang/compare_nan.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-compare COMMAND cobalt -e "_U=true" lang/compare.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-concat COMMAND cobalt -e "_U=true" lang/concat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-for COMMAND cobalt -e "_U=true" lang/for.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-gc COMMAND cobalt -e "_U=true" lang/gc.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-length COMMAND cobalt -e "_U=true" lang/length.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-self COMMAND cobalt -e "_U=true" lang/self.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-table COMMAND cobalt -e "_U=true" lang/table.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-tail_recursion COMMAND cobalt -e "_U=true" lang/tail_recursion.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-constant-number COMMAND cobalt -e "_U=true" lang/constant/number.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-constant-table COMMAND cobalt -e "_U=true" lang/constant/table.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-arith COMMAND cobalt -e "_U=true" lang/meta/arith.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-cat COMMAND cobalt -e "_U=true" lang/meta/cat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-eq COMMAND cobalt -e "_U=true" lang/meta/eq.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-framegap COMMAND cobalt -e "_U=true" lang/meta/framegap.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-newindex COMMAND cobalt -e "_U=true" lang/meta/newindex.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-nomm COMMAND cobalt -e "_U=true" lang/meta/nomm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-upvalue-closure COMMAND cobalt -e "_U=true" lang/upvalue/closure.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)

    # bench
    ## mandelbrot.cobalt
    ## spectral-norm.cobalt
    ## scimark-sor.cobalt
    ## meteor.cobalt
    ## series.cobalt
    ## md5.cobalt
    ## sum-file.cobalt
    ## recursive-ack.cobalt
    ## array3d.cobalt
    ## fasta.cobalt
    ## fannkuch.cobalt
    ## coroutine-ring.cobalt
    ## life.cobalt
    ## scimark-fft.cobalt
    ## k-nucleotide.cobalt
    ## pidigits-nogmp.cobalt
    ## euler14-bit.cobalt
    ## scimark-lu.cobalt
    ## partialsums.cobalt
    ## binary-trees.cobalt
    ## ray.cobalt
    ## scimark-sparse.cobalt
    ## nsieve-bit.cobalt
    ## revcomp.cobalt
    ## nbody.cobalt
    ## chameneos.cobalt
    ## mandelbrot-bit.cobalt
    ## recursive-fib.cobalt
    ## scimark-2010-12-20.cobalt
    ## scimark_lib.cobalt
    ## nsieve-bit-fp.cobalt
    ## nsieve.cobalt
    ## roulette.cobalt

    add_test(NAME bench-mandelbrot COMMAND cobalt -e "_U=true" bench/mandelbrot.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-spectral-norm COMMAND cobalt -e "_U=true" bench/spectral-norm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark-sor COMMAND cobalt -e "_U=true" bench/scimark-sor.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-meteor COMMAND cobalt -e "_U=true" bench/meteor.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-series COMMAND cobalt -e "_U=true" bench/series.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-md5 COMMAND cobalt -e "_U=true" bench/md5.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-sum-file COMMAND cobalt -e "_U=true" bench/sum-file.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-recursive-ack COMMAND cobalt -e "_U=true" bench/recursive-ack.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-array3d COMMAND cobalt -e "_U=true" bench/array3d.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-fasta COMMAND cobalt -e "_U=true" bench/fasta.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-fannkuch COMMAND cobalt -e "_U=true" bench/fannkuch.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-coroutine-ring COMMAND cobalt -e "_U=true" bench/coroutine-ring.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-life COMMAND cobalt -e "_U=true" bench/life.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark-fft COMMAND cobalt -e "_U=true" bench/scimark-fft.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-k-nucleotide COMMAND cobalt -e "_U=true" bench/k-nucleotide.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-pidigits-nogmp COMMAND cobalt -e "_U=true" bench/pidigits-nogmp.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-euler14-bit COMMAND cobalt -e "_U=true" bench/euler14-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark-lu COMMAND cobalt -e "_U=true" bench/scimark-lu.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-partialsums COMMAND cobalt -e "_U=true" bench/partialsums.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-binary-trees COMMAND cobalt -e "_U=true" bench/binary-trees.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-ray COMMAND cobalt -e "_U=true" bench/ray.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark-sparse COMMAND cobalt -e "_U=true" bench/scimark-sparse.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve-bit COMMAND cobalt -e "_U=true" bench/nsieve-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-revcomp COMMAND cobalt -e "_U=true" bench/revcomp.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nbody COMMAND cobalt -e "_U=true" bench/nbody.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-chameneos COMMAND cobalt -e "_U=true" bench/chameneos.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-mandelbrot-bit COMMAND cobalt -e "_U=true" bench/mandelbrot-bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-recursive-fib COMMAND cobalt -e "_U=true" bench/recursive-fib.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark-2010-12-20 COMMAND cobalt -e "_U=true" bench/scimark-2010-12-20.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-scimark_lib COMMAND cobalt -e "_U=true" bench/scimark_lib.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve-bit-fp COMMAND cobalt -e "_U=true" bench/nsieve-bit-fp.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-nsieve COMMAND cobalt -e "_U=true" bench/nsieve.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME bench-roulette COMMAND cobalt -e "_U=true" bench/roulette.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    
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