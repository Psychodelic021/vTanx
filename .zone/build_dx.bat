@echo off

:: Set up the Visual Studio environment
:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
:: if errorlevel 1 (
    ::echo Failed to set up Visual Studio environment.
    ::exit /b 1
::)

:: Compiler options
SET COMPILER=cl
SET CFLAGS=/Zi /EHsc /W4 /std:c++20 /fsanitize=address /MDd
SET DEFINES=/D_DEBUG
SET INCLUDES=/Isource /Ivendor
SET SOURCE=source/zmain.cc

:: Linker options
SET LINKER=/link
SET LFLAGS=/DEBUG /MACHINE:X64 /SUBSYSTEM:CONSOLE
SET LIBS=
SET TARGET=/OUT:engine.exe

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE% %LINKER% %LFLAGS% %LIBS% %TARGET%

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build succeeded.