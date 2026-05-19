@echo off
C:\msys64\ucrt64\bin\g++.exe main.cpp -o main.exe -IC:\msys64\ucrt64\include -LC:\msys64\ucrt64\lib -lfreeglut -lopengl32 -lglu32
if %errorlevel% neq 0 (
    echo Compile failed.
    pause
    exit /b
)
.\main.exe
pause