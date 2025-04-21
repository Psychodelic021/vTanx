@echo off
echo -------------------------------------------
echo Building 3D Game Engine with Vulkan
echo -------------------------------------------

:: Compiler options
SET COMPILER=clang++.exe
SET CFLAGS= -g -std=c++20 -Wno-missing-braces
SET DEFINES=-D_DEBUG -DDEBUG
SET INCLUDES=-Isrc -I%VULKAN_SDK%\Include -Ivendor
SET SOURCE=src/engine.cpp

:: Linker options
SET LIBS=-L%VULKAN_SDK%\Lib -lvulkan-1 -luser32 -lshell32 -lgdi32 -lmsvcrtd -Wl,/NODEFAULTLIB:libcmt
SET TARGET=engine.exe

:: Show build command
echo Building with command:
echo %COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE% %LIBS% -o %TARGET%

:: Build
%COMPILER% %CFLAGS% %DEFINES% %INCLUDES% %SOURCE% %LIBS% -o %TARGET%

:: Check for errors
IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed with error code %ERRORLEVEL%
    echo See build errors above
    exit /b %ERRORLEVEL%
) ELSE (
    echo.
    echo Build successful! Created %TARGET%
    echo File size: 
    for %%A in (%TARGET%) do echo %%~zA bytes
)

exit /b 0