@echo off
setlocal

rem Set the path to the Visual Studio environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

rem Set the source file and output library name
set SRC_FILE=glad.c
set OUT_LIB=glad_debug.lib

rem Compile the source file into a static library
cl /Zi /c /I. %SRC_FILE%
lib /OUT:%OUT_LIB% glad.obj

rem Clean up the object file
del glad.obj

rem Set the source file and output library name
set SRC_FILE=glad_wgl.c
set OUT_LIB=glad_wgl_debug.lib

rem Compile the source file into a static library
cl /Zi /c /I. %SRC_FILE%
lib /OUT:%OUT_LIB% glad_wgl.obj

rem Clean up the object files
del glad_wgl.obj

echo Build complete.

endlocal