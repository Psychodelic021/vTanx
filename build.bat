@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Compiler options
SET COMPILER=cl
SET CFLAGS=/Zi /Oi /W4 /fsanitize=address /std:c17
SET DEFINES=/D DEBUG /D _CRT_SECURE_NO_WARNINGS 
SET INCLUDES=/Isrc /I%VULKAN_SDK%\Include
SET SOURCE=src/vtanx.c

:: Linker options
SET LINKER=/link
SET LFLAGS=/DEBUG /MACHINE:X64 /SUBSYSTEM:CONSOLE
SET LIBS=/LIBPATH:%VULKAN_SDK%\Lib vulkan-1.lib user32.lib

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE%   %LINKER% %LFLAGS% %LIBS%
