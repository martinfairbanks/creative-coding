REM If you want to build with the MSVC compiler you have to execute the vcvarsall.bat batch file:
REM 64-bit
REM call "c:\program files (x86)\microsoft visual studio 14.0\vc\vcvarsall.bat" x64
REM 32-bit
REM call "c:\program files (x86)\microsoft visual studio 14.0\vc\vcvarsall.bat"

@echo off
set CompilerFlags=-EHsc -nologo -O2 -Oi -fp:fast
set LinkerFlags=-link -incremental:no -opt:ref -SUBSYSTEM:WINDOWS

IF NOT EXIST build mkdir build
pushd build

cl %CompilerFlags% ../framework_demo/dynamic_array.cpp %LinkerFlags%
cl %CompilerFlags% ../framework_demo/test_ogl_sdl_01.cpp %LinkerFlags%
cl %CompilerFlags% ../framework_demo/test_ogl_sdl_02.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/bouncing_balls.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/rhodonea_curve01.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/rhodonea_curve02.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/rhodonea_curve03.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/sinescaling.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/sinewaves.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/snow.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/sprites.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/stars2d.cpp %LinkerFlags%
cl %CompilerFlags% ../examples/stars3d.cpp %LinkerFlags%

del *.exp
del *.lib
del *.obj

popd