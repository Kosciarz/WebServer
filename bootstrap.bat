@echo off
REM Bootstrap script to initialize and update submodules, and install dependencies

REM Ensure we are in the root directory of the project
cd /d "%~dp0"

REM Initialize git submodules
echo Initializing submodules...
git submodule update --init --recursive

REM Bootstrap vcpkg (if not already bootstrapped)
IF NOT EXIST "external\vcpkg\vcpkg.exe" (
    echo Bootstrapping vcpkg...
    cd external\vcpkg
    call bootstrap-vcpkg.bat
) ELSE (
    echo vcpkg is already bootstrapped.
)

REM Install dependencies using vcpkg (example: Boost)
echo Installing dependencies with vcpkg...
call .\vcpkg install boost-system boost-asio 
cd ..

REM Return to the root directory
echo Bootstrap complete. Proceed to the build steps.