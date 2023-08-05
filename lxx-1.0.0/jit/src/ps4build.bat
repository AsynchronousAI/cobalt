@rem Script to build LuaJIT with the PS4 SDK.
@rem Donated to the public domain.
@rem
@rem Open a "Visual Studio .NET Command Prompt" (64 bit host compiler)
@rem or "VS2015 x64 Native Tools Command Prompt".
@rem
@rem Then cd to this directory and run this script.
@rem
@rem Recommended invocation:
@rem
@rem ps4build        release build, amalgamated, 64-bit GC
@rem ps4build debug    debug build, amalgamated, 64-bit GC
@rem
@rem Additional command-line options (not generally recommended):
@rem
@rem gc32 (before debug)    32-bit GC
@rem noamalg (after debug)  non-amalgamated build

@if not defined INCLUDE goto :FAIL
@if not defined SCE_ORBIS_SDK_DIR goto :FAIL

@setlocal
@rem ---- Host compiler ----
@set LJCOMPILE=cl /nologo /c /MD /O2 /W3 /D_CRT_SECURE_NO_DEPRECATE
@set LJLINK=link /nologo
@set LJMT=mt /nologo
@set DASMDIR=..\dynasm
@set DASM=%DASMDIR%\dynasm.
@set ALL_LIB=lib_base.c lib_math.c lib_bit.c lib_string.c lib_table.c lib_io.c lib_os.c lib_package.c lib_debug.c lib_jit.c lib_ffi.c
@set GC64=-DLUAJIT_ENABLE_GC64
@set DASC=vm_x64.dasc

@if "%1" neq "gc32" goto :NOGC32
@shift
@set GC64=
@set DASC=vm_x86.dasc
:NOGC32

%LJCOMPILE% host\mini.c
@if errorlevel 1 goto :BAD
%LJLINK% /out:mini.exe mini.obj
@if errorlevel 1 goto :BAD
if exist mini.exe.manifest^
  %LJMT% -manifest mini.exe.manifest -outputresource:mini.exe

@rem Check for 64 bit host compiler.
@mini
@if not errorlevel 8 goto :FAIL

@set DASMFLAGS=-D P64 -D NO_UNWIND
mini %DASM% -LN %DASMFLAGS% -o host\buildvm_arch.h %DASC%
@if errorlevel 1 goto :BAD

%LJCOMPILE% /I "." /I %DASMDIR% %GC64% -DLUAJIT_TARGET=LUAJIT_ARCH_X64 -DLUAJIT_OS=LUAJIT_OS_OTHER -DLUAJIT_DISABLE_JIT -DLUAJIT_DISABLE_FFI -DLUAJIT_NO_UNWIND host\buildvm*.c
@if errorlevel 1 goto :BAD
%LJLINK% /out:buildvm.exe buildvm*.obj
@if errorlevel 1 goto :BAD
if exist buildvm.exe.manifest^
  %LJMT% -manifest buildvm.exe.manifest -outputresource:buildvm.exe

buildvm -m elfasm -o lj_vm.s
@if errorlevel 1 goto :BAD
buildvm -m bcdef -o lj_bcdef.h %ALL_LIB%
@if errorlevel 1 goto :BAD
buildvm -m ffdef -o lj_ffdef.h %ALL_LIB%
@if errorlevel 1 goto :BAD
buildvm -m libdef -o lj_libdef.h %ALL_LIB%
@if errorlevel 1 goto :BAD
buildvm -m recdef -o lj_recdef.h %ALL_LIB%
@if errorlevel 1 goto :BAD
buildvm -m vmdef -o jit\vmdef. %ALL_LIB%
@if errorlevel 1 goto :BAD
buildvm -m folddef -o lj_folddef.h lj_opt_fold.c
@if errorlevel 1 goto :BAD

@rem ---- Cross compiler ----
@set LJCOMPILE="%SCE_ORBIS_SDK_DIR%\host_tools\bin\orbis-clang" -c -Wall -DLUAJIT_DISABLE_FFI %GC64%
@set LJLIB="%SCE_ORBIS_SDK_DIR%\host_tools\bin\orbis-ar" rcus
@set INCLUDE=""

orbis-as -o lj_vm.o lj_vm.s

@if "%1" neq "debug" goto :NODEBUG
@shift
@set LJCOMPILE=%LJCOMPILE% -g -O0
@set TARGETLIB=libluajitD_ps4.a
goto :BUILD
:NODEBUG
@set LJCOMPILE=%LJCOMPILE% -O2
@set TARGETLIB=libluajit_ps4.a
:BUILD
del %TARGETLIB%
@if "%1" neq "noamalg" goto :AMALG
for %%f in (lj_*.c lib_*.c) do (
  %LJCOMPILE% %%f
  @if errorlevel 1 goto :BAD
)

%LJLIB% %TARGETLIB% lj_*.o lib_*.o
@if errorlevel 1 goto :BAD
@goto :NOAMALG
:AMALG
%LJCOMPILE% ljamalg.c
@if errorlevel 1 goto :BAD
%LJLIB% %TARGETLIB% ljamalg.o lj_vm.o
@if errorlevel 1 goto :BAD
:NOAMALG

@del *.o *.obj *.manifest mini.exe buildvm.exe
@echo.
@echo === Successfully built LuaJIT for PS4 ===

@goto :END
:BAD
@echo.
@echo *******************************************************
@echo *** Build FAILED -- Please check the error messages ***
@echo *******************************************************
@goto :END
:FAIL
@echo To run this script you must open a "Visual Studio .NET Command Prompt"
@echo (64 bit host compiler). The PS4 Orbis SDK must be installed, too.
:END