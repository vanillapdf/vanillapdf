:: ------------- win-x86_v143 -------------
mkdir "build\win-x86_v143"
cd "build\win-x86_v143"

call cmake -G "Visual Studio 17 2022" -DCMAKE_GENERATOR_PLATFORM=Win32 -DCMAKE_GENERATOR_TOOLSET=v143 ..\..\..
call cmake --build . --config Release --target ALL_BUILD
call cmake --build . --config Release --target RUN_TESTS
call cmake --build . --config Release --target PACKAGE

cd ..\..
:: ------------- win-x86_v143 -------------

:: ------------- win-x64_v143 -------------
mkdir "build\win-x64_v143"
cd "build\win-x64_v143"

call cmake -G "Visual Studio 17 2022" -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_GENERATOR_TOOLSET=v143 ..\..\..
call cmake --build . --config Release --target ALL_BUILD
call cmake --build . --config Release --target RUN_TESTS
call cmake --build . --config Release --target PACKAGE

cd ..\..
:: ------------- win-x64_v143 -------------

:: ------------- NUGET_FINAL -------------
::rm vanillapdf.nupkg
nuget pack vanillapdf.nuspec
:: ------------- NUGET_FINAL -------------
