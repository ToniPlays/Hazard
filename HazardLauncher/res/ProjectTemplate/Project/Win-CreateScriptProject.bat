@echo off
pushd %~dp0
cd ../
call %HAZARD_DIR%\vendor\premake\bin\premake5.exe vs2022 %*
popd