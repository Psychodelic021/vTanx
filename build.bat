@echo off

:: call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Flags
SET CC=cl
SET FLAGS=/EHsc /Zi
SET INCLUDES=/Isrc /I%VULKAN_SDK%\Include
SET DEFINES=/D DEBUG
SET SRC=src/main.c
SET LINKS=/link /LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib

:: Compile
%CC% %FLAGS% %INCLUDES% %SRC% %LINKS% 
