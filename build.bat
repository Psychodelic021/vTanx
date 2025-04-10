@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Compiler options
SET COMPILER=clang.exe
SET CFLAGS= -g -std=c17 -Wvarargs -Wall -Wextra -Wno-missing-braces -Wno-unused-parameter -Wno-unused-variable 
SET DEFINES=-D_DEBUG -DDEBUG
SET INCLUDES=-Isource -I%VULKAN_SDK%\Include 
SET SOURCE=source/zx_engine.c

:: Linker options
SET LIBS=-L%VULKAN_SDK%\Lib 
SET TARGET=zxengine.exe

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE%   %LIBS% -o %TARGET%

IF ERRORLEVEL 1 (
    echo Build failed.
    exit /b 1
) ELSE (
    echo Build succeeded.
)

:: Run the Program
:: echo Running the program...
:: zxengine.exe