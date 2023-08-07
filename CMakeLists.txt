cmake_minimum_required(VERSION 3.1)
project(cobalt LANGUAGES C VERSION 1.0.0)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" OFF)
option(LUA_BUILD_AS_CXX "Build cobalt as C++" OFF)
option(LUA_ENABLE_SHARED "Build dynamic liblua" ON)
option(LUA_ENABLE_TESTING "Build and run tests" ON)
option(INSTALL "Install binaries to system" OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
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
    option(LUA_BUILD_COMPILER "Build luac compiler" ON)
else()
    option(LUA_BUILD_BINARY "Build cobalt binary" OFF)
    option(LUA_BUILD_COMPILER "Build luac compiler" ON)
endif()

add_subdirectory(cobalt-1.0.0)
add_custom_target(lua-cobalt
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/lua-cobalt
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building lua-cobalt"
)
add_custom_target(jit
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-msvc
    COMMAND bat msvcbuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-xbox
    COMMAND bat xb1build.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-xbox360
    COMMAND bat xedkbuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-ps5
    COMMAND bat ps5build.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-ps4
    COMMAND bat ps4build.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-vita
    COMMAND bat psvitabuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-switch
    COMMAND bat nxbuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt-1.0.0/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
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