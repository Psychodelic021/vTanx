@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Compiler options
SET COMPILER=cl
SET CFLAGS=/Zi /W4 /fsanitize=address
SET DEFINES=/D_DEBUG /D_CRT_SECURE_NO_WARNINGS
SET INCLUDES=/Isrc /I%VULKAN_SDK%\Include
SET SOURCE=src/main.cc

:: Linker options
SET LINKER=/link
SET LFLAGS=/DEBUG /MACHINE:X64 /SUBSYSTEM:CONSOLE
SET LIBS=/LIBPATH:%VULKAN_SDK%\Lib vulkan-1.lib user32.lib
SET TARGET=/OUT:vtanx.exe

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE%   %LINKER% %LFLAGS% %LIBS% %TARGET%
