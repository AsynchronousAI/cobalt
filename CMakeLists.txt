cmake_minimum_required(VERSION 3.1)
project(lxx LANGUAGES C VERSION 1.0.0)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" OFF)
option(LUA_BUILD_AS_CXX "Build lxx as C++" OFF)
option(LUA_ENABLE_SHARED "Build dynamic liblua" ON)
option(LUA_ENABLE_TESTING "Build and run tests" ON)
option(INSTALL "Install binaries to system" OFF)
enable_language(CXX)
if(LUA_ENABLE_TESTING)
    enable_testing()
endif()

if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
    set(TOP_LEVEL TRUE)
else()
    set(TOP_LEVEL FALSE)
endif()
add_custom_target(lua-lxx
    COMMAND make ${CMAKE_CURRENT_SOURCE_DIR}/lxx-${CMAKE_PROJECT_VERSION}/lua-lxx
    COMMENT "Building lua-lxx"
)
add_custom_target(install
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/install.py
    COMMENT "Installing"
)
if(TOP_LEVEL)
    option(LUA_BUILD_BINARY "Build lxx binary" ON)
    option(LUA_BUILD_COMPILER "Build luac compiler" ON)
else()
    option(LUA_BUILD_BINARY "Build lxx binary" OFF)
    option(LUA_BUILD_COMPILER "Build luac compiler" ON)
endif()

add_subdirectory(lxx-1.0.0)

if(LUA_ENABLE_TESTING)
    add_test(NAME lua-testsuite COMMAND lxx -e "_U=true" all.lxx WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lxx-${CMAKE_PROJECT_VERSION}-tests)
endif()

if (INSTALL)
    add_custom_target(my_target
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/install.py
        COMMENT "Installing binaries"
    )
endif()