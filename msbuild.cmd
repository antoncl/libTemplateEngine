@echo off

:NOBUILD
IF "%1"=="" goto NMAKE
IF "%1"=="nmake" goto NMAKE
IF "%1"=="vs" goto VSTUDIO
echo "Usage: %0 nmake"
echo "         - build using nmake"
echo "       %0 vs"
echo "         - build using a visual studio 2015 solution"
GOTO EXIT
:NMAKE
rem Build using nmake
rem Build the debug version
rem
call :Cleanup
echo Building via nmake
mkdir build
mkdir build\debug
cd build\debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=..\.. -G"NMake Makefiles" ..\..\src
cd ..\..
cmake --build build\debug --config Debug
cmake --build build\debug --target install

rem
rem Build the release version
rem
mkdir build\release
cd build\release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=..\.. -G"NMake Makefiles" ..\..\src
cd ..\..
cmake --build build\release --config Release
cmake --build build\release --target install
GOTO EXIT

:VSTUDIO
rem
rem Build using visual studio
rem
call :Cleanup
echo Building via Visual Studio 
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=.. -G"Visual Studio 14 2015 Win64" ..\src
cd ..
cmake --build build --config Release --target install
cmake --build build --config Debug --target install
GOTO EXIT

:Cleanup
IF NOT EXIST build GOTO NOBUILD
	echo Deleting old configuration
	rmdir /S /Q build
exit /b


:EXIT