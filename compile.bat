@echo off
::if "%1"=="-c" (echo "YES")
::if NOT "%1"=="-c" (echo "NO")
if NOT "%1"=="-c" (cls)
make
if NOT "%1"=="-c" (pause)
exit