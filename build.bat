@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Compiler options
SET COMPILER=cl
SET CFLAGS=/Zi /EHsc /W4 /std:c++20 /fsanitize=address /MDd
SET DEFINES=/D_DEBUG
SET INCLUDES=/Isource
SET SOURCE=source/main.cc

:: Linker options
SET LINKER=/link
SET LFLAGS=/DEBUG /MACHINE:X64 /SUBSYSTEM:CONSOLE
SET LIBS=user32.lib d3d11.lib dxguid.lib d3dcompiler.lib
SET TARGET=/OUT:engine.exe

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE%   %LINKER% %LFLAGS% %LIBS% %TARGET%
    