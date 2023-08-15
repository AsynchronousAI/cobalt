cmake_minimum_required(VERSION 3.1)
project(cobalt LANGUAGES C VERSION 23)

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

add_subdirectory(cobalt23)
# CLEAR
add_custom_target(cleanjit
    COMMAND make -w clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Clearing cobalt23"
)
add_custom_target(cleanraptor
    COMMAND make -w clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit
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
add_custom_target(jit
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(raptor
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/raptor
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltRaptor (this may take a while)"
)
# WINDOWS BUILD JIT CONSOLES
add_custom_target(jit-xbox
    COMMAND start xb1build.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-xbox360
    COMMAND start xedkbuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-ps5
    COMMAND start ps5build.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-ps4
    COMMAND start ps4build.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-vita
    COMMAND start psvitabuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-switch
    COMMAND start nxbuild.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)

# POSIX BUILD JIT CONSOLES
add_custom_target(jit-xbox-posix
    COMMAND ./xb1build.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-xbox360-posix
    COMMAND ./xedkbuild.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-ps5-posix
    COMMAND ./ps5build.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-ps4-posix
    COMMAND ./ps4build.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-vita-posix
    COMMAND ./psvitabuild.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building cobaltJIT (this may take a while)"
)
add_custom_target(jit-switch-posix
    COMMAND ./nxbuild.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cobalt23/jit/src
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