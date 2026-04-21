@echo off

:: Generate Solution (and Projects) file
cmake -S . -B build

:: opening .slnx (or .sln file) if Visual Studio is closed
tasklist | find /I "devenv.exe" >nul && exit /b
for %%f in (build\ETW.sln build\ETW.slnx) do if exist "%%f" (
    start "" "%%f"
    exit /b
)