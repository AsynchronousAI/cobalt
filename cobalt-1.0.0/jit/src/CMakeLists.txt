cmake_minimum_required(VERSION 3.0)
project(LuaJIT)

set(MAJVER 2)
set(MINVER 1)
set(RELVER 0)
set(ABIVER 5.1)
set(NODOTABIVER 51)

# Compiler options
set(DEFAULT_CC gcc)
set(CC ${DEFAULT_CC})
set(CCOPT -O2 -fomit-frame-pointer)
set(CCWARN -Wall)
set(CCDEBUG "")
set(XCFLAGS -DLUAJIT_LJS)
set(XCFLAGS -DLUAJIT_ENABLE_LUA52COMPAT)
set(BUILDMODE mixed)
set(TARGET_SYS ${CMAKE_SYSTEM_NAME})

# ... (other variable definitions)

if(TARGET_SYS STREQUAL "Windows")
  set(TARGET_STRIP "")
  set(TARGET_XSHLDFLAGS "-shared -Wl,--out-implib,${TARGET_DLLDOTANAME}")
  set(TARGET_DYNXLDOPTS "")
  set(TARGET_TCFLAGS ${CCOPTIONS} ${TARGET_XCFLAGS} ${TARGET_FLAGS} ${TARGET_CFLAGS})
  set(TARGET_ACFLAGS ${CCOPTIONS} ${TARGET_XCFLAGS} ${TARGET_FLAGS} ${TARGET_CFLAGS})
  set(TARGET_ASFLAGS ${ASOPTIONS} ${TARGET_XCFLAGS} ${TARGET_FLAGS} ${TARGET_CFLAGS})
  set(TARGET_ALDFLAGS ${LDOPTIONS} ${TARGET_XLDFLAGS} ${TARGET_FLAGS} ${TARGET_LDFLAGS})
  set(TARGET_ASHLDFLAGS ${LDOPTIONS} ${TARGET_XSHLDFLAGS} ${TARGET_FLAGS} ${TARGET_SHLDFLAGS})
  set(TARGET_ALIBS ${TARGET_XLIBS} ${LIBS} ${TARGET_LIBS})
endif()

# ... (other build configurations)

add_executable(luajit ${LUAJIT_O})
target_compile_options(luajit PRIVATE ${TARGET_TCFLAGS})
target_link_libraries(luajit PRIVATE ${TARGET_ALIBS})

add_library(libluajit STATIC ${LJVMCORE_O})
target_compile_options(libluajit PRIVATE ${TARGET_TCFLAGS})
target_link_libraries(libluajit PRIVATE ${TARGET_ALIBS})

add_library(libluajit_shared SHARED ${LJVMCORE_DYNO})
set_target_properties(libluajit_shared PROPERTIES OUTPUT_NAME libluajit)
target_compile_options(libluajit_shared PRIVATE ${TARGET_TCFLAGS})
target_link_libraries(libluajit_shared PRIVATE ${TARGET_ALIBS} ${TARGET_DYNXLDOPTS})

# ... (other targets)

# Additional cmake setup goes here, if needed
