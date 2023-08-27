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

    add_test(NAME lib-bit COMMAND cobalt -e "_U=true" lib/bit.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-concat COMMAND cobalt -e "_U=true" lib/table/concat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-insert COMMAND cobalt -e "_U=true" lib/table/insert.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-misc COMMAND cobalt -e "_U=true" lib/table/misc.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-new COMMAND cobalt -e "_U=true" lib/table/new.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-pack COMMAND cobalt -e "_U=true" lib/table/pack.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-remove COMMAND cobalt -e "_U=true" lib/table/remove.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-table-sort COMMAND cobalt -e "_U=true" lib/table/sort.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-format-num COMMAND cobalt -e "_U=true" lib/string/format/num.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-byte COMMAND cobalt -e "_U=true" lib/string/byte.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-char COMMAND cobalt -e "_U=true" lib/string/char.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-dump COMMAND cobalt -e "_U=true" lib/string/dump.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-len COMMAND cobalt -e "_U=true" lib/string/len.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-string-lower-upper COMMAND cobalt -e "_U=true" lib/string/lower-upper.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
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
    add_test(NAME lib-base-getfenv COMMAND cobalt -e "_U=true" lib/base/getfenv.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-getsetmetatable COMMAND cobalt -e "_U=true" lib/base/getsetmetatable.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-ipairs COMMAND cobalt -e "_U=true" lib/base/ipairs.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-next COMMAND cobalt -e "_U=true" lib/base/next.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-pairs COMMAND cobalt -e "_U=true" lib/base/pairs.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-select COMMAND cobalt -e "_U=true" lib/base/select.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lib-base-tonumber-tostring COMMAND cobalt -e "_U=true" lib/base/tonumber-tostring.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    
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
    add_test(NAME lang-coroutine COMMAND cobalt -e "_U=true" lang/coroutine.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-for COMMAND cobalt -e "_U=true" lang/for.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-gc COMMAND cobalt -e "_U=true" lang/gc.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-goto COMMAND cobalt -e "_U=true" lang/goto.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-length COMMAND cobalt -e "_U=true" lang/length.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-modulo COMMAND cobalt -e "_U=true" lang/modulo.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-self COMMAND cobalt -e "_U=true" lang/self.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-table COMMAND cobalt -e "_U=true" lang/table.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-tail_recursion COMMAND cobalt -e "_U=true" lang/tail_recursion.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-constant-number COMMAND cobalt -e "_U=true" lang/constant/number.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-constant-table COMMAND cobalt -e "_U=true" lang/constant/table.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-arith COMMAND cobalt -e "_U=true" lang/meta/arith.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-call COMMAND cobalt -e "_U=true" lang/meta/call.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-cat COMMAND cobalt -e "_U=true" lang/meta/cat.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-comp COMMAND cobalt -e "_U=true" lang/meta/comp.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-debuginfo COMMAND cobalt -e "_U=true" lang/meta/debuginfo.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-eq COMMAND cobalt -e "_U=true" lang/meta/eq.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-framegap COMMAND cobalt -e "_U=true" lang/meta/framegap.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-index COMMAND cobalt -e "_U=true" lang/meta/index.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-len COMMAND cobalt -e "_U=true" lang/meta/len.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-newindex COMMAND cobalt -e "_U=true" lang/meta/newindex.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-meta-nomm COMMAND cobalt -e "_U=true" lang/meta/nomm.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME lang-upvalue-closure COMMAND cobalt -e "_U=true" lang/upvalue/closure.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)

    # common
    ## expect_error.cobalt
    ## test_runner_canary.cobalt

    add_test(NAME common-expect_error COMMAND cobalt -e "_U=true" common/expect_error.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
    add_test(NAME common-test_runner_canary COMMAND cobalt -e "_U=true" common/test_runner_canary.cobalt WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23-tests)
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