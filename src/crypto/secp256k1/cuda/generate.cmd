@echo off
REM Usage: generate.cmd <boost-include-directory>
setlocal
set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`%VSWHERE% -latest -property installationPath`) do set VS=%%i
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul
cd /d "%~dp0"
"%VS%\VC\Tools\Llvm\x64\bin\clang++.exe" -x cuda --cuda-device-only ^
    --cuda-gpu-arch=sm_75 -nocudainc -nocudalib -std=c++20 -O3 -S ^
    "-D__host__=__attribute__((host))" ^
    "-D__device__=__attribute__((device))" ^
    "-D__global__=__attribute__((global))" ^
    -DNDEBUG -DBOOST_ALL_NO_LIB -DBOOST_URL_HPP -D_WIN32_WINNT=0x0A00 ^
    -I ..\..\..\..\include -I ..\..\..\..\build\include -I "%~1" ^
    verify.cu -o verify.ptx
