@echo off

set file_long=%1
set file=%file_long:~4,-2%.exe
call gcc -I.\include -L.\lib\win\ %1 -lglfw3 -lgdi32 -o %file%
if not exist "out" mkdir out
move .\%file% .\out\%file% >nul
if "%2"=="--run" call .\out\%file%