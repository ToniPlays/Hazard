@echo off
pushd %~dp0
cd ../
call C:\dev\Hazard\vendor\premake\bin\premake5.exe vs2022 %*
popd