@echo off

rem
rem Build using visual studio
rem

rem cleanup remnants of last build
IF NOT EXIST build GOTO BUILD
echo Deleting old configuration
rmdir /S /Q build

:BUILD
echo Building via Visual Studio 
mkdir build & pushd build

rem build 32 bit version
mkdir x86 & pushd x86
cmake -DCMAKE_INSTALL_PREFIX=../.. -G"Visual Studio 14 2015" ..\..\src
popd
cmake --build x86 --config Release --target install
cmake --build x86 --config Debug --target install

rem build 64 bit version
mkdir x64 & pushd x64
cmake -DCMAKE_INSTALL_PREFIX=../.. -G"Visual Studio 14 2015 Win64" ..\..\src
popd
cmake --build x64 --config Release --target install
cmake --build x64 --config Debug --target install

popd