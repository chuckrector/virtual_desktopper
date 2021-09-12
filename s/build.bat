@echo off

set CompilerFlags=/GS /WX /W4 /wd4100 /wd4189 /wd4505 /wd4576 /wd4702 /wd4706 /wd4996 /FC /Zi /Od /Oi
set LinkerFlags=/opt:ref user32.lib gdi32.lib ole32.lib
REM  gdi32.lib winmm.lib

mkdir build
pushd build

REM cl %CompilerFlags% ..\code\virtual_desktopper.c
@REM cl %CompilerFlags% ..\code\virtual_desktopper.c /E /link %LinkerFlags% > preproc.txt
cl %CompilerFlags% ..\virtual_desktopper.c /link %LinkerFlags%

popd