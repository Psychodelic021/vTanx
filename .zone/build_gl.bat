@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Compiler options
SET COMPILER=cl
SET CFLAGS=/Zi /MP /EHsc /W4 /fsanitize=address /std:c17
SET DEFINES=/D_DEBUG /D_CRT_SECURE_NO_WARNINGS
SET INCLUDES=/Ivendor
SET SOURCE=src/vtanx.c

:: Linker options
SET LINKER=/link
SET LFLAGS=/DEBUG /MACHINE:X64 /SUBSYSTEM:CONSOLE
SET LIBS=gdi32.lib opengl32.lib vendor/glad_debug.lib vendor/glad_wgl_debug.lib

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE%   %LINKER% %LFLAGS% %LIBS%
