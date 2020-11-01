:: ------------- win-x86_v140 -------------
mkdir "build\win-x86_v140"
cd "build\win-x86_v140"

call cmake -G "Visual Studio 14 2015" -DCMAKE_GENERATOR_PLATFORM=Win32 -DCMAKE_GENERATOR_TOOLSET=v140 ..\..\..
call cmake --build . --config Release --target ALL_BUILD
call cmake --build . --config Release --target RUN_TESTS
call cmake --build . --config Release --target PACKAGE

cd ..\..
:: ------------- win-x86_v140 -------------

:: ------------- win-x64_v140 -------------
mkdir "build\win-x64_v140"
cd "build\win-x64_v140"

call cmake -G "Visual Studio 14 2015" -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_GENERATOR_TOOLSET=v140 ..\..\..
call cmake --build . --config Release --target ALL_BUILD
call cmake --build . --config Release --target RUN_TESTS
call cmake --build . --config Release --target PACKAGE

cd ..\..
:: ------------- win-x64_v140 -------------

:: ------------- NUGET_FINAL -------------
::rm vanillapdf.nupkg
nuget pack vanillapdf.nuspec
:: ------------- NUGET_FINAL -------------
