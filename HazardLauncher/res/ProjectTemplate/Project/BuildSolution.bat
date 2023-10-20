@echo off
"%ProgramFiles%\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" %CSPROJ_PATH%.csproj -property:Configuration=Debug