@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Compiler options
SET COMPILER=cl
SET CFLAGS=/nologo /Zi /W4 /std:clatest /fsanitize=address /MDd
SET DEFINES=/D_DEBUG
SET INCLUDES=/Isource /I%VULKAN_SDK%\Include
SET SOURCE=source/zx_engine.c

:: Linker options
SET LINKER=/link
SET LFLAGS=/DEBUG /MACHINE:X64 /SUBSYSTEM:CONSOLE
SET LIBS=/LIBPATH:%VULKAN_SDK%\Lib 
SET TARGET=/OUT:zxengine.exe

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE%   %LINKER% %LFLAGS% %LIBS% %TARGET%

IF ERRORLEVEL 1 (
    echo Build failed.
    exit /b 1
) ELSE (
    echo Build succeeded.
)

:: Run the Program
:: echo Running the program...
:: engine.exe