cmake_minimum_required(VERSION 3.1)
project(lxx LANGUAGES C VERSION 1.0.0)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" OFF)
option(LUA_BUILD_AS_CXX "Build lxx as C++" OFF)
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
    option(LUA_BUILD_BINARY "Build lxx binary" ON)
    option(LUA_BUILD_COMPILER "Build luac compiler" ON)
else()
    option(LUA_BUILD_BINARY "Build lxx binary" OFF)
    option(LUA_BUILD_COMPILER "Build luac compiler" ON)
endif()

add_subdirectory(lxx-1.0.0)
add_custom_target(lua-lxx
    COMMAND make -w
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lxx-1.0.0/lua-lxx
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Makefile
    COMMENT "Building lua-lxx"
)
if(LUA_ENABLE_TESTING)
    add_test(NAME lua-testsuite COMMAND lxx -e "_U=true" all.lxx WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lxx-${CMAKE_PROJECT_VERSION}-tests)
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