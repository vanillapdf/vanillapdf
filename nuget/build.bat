SET var=%cd%
set msbuild="C:/Program Files (win-86)/MSBuild/14.0/Bin/msbuild.exe"

::mkdir ..\%dstnuget%\build\native\include
::copy jconfig.h ..\%dstnuget%\build\native\include
::copy jconfigint.h ..\%dstnuget%\build\native\include
::copy libjpeg-turbo\turbojpeg.h ..\%dstnuget%\build\native\include
::copy libjpeg-turbo\jmorecfg.h ..\%dstnuget%\build\native\include
::copy libjpeg-turbo\jpeglib.h ..\%dstnuget%\build\native\include
::copy libjpeg-turbo\jerror.h ..\%dstnuget%\build\native\include

:: ------------- win-86_v140 -------------
mkdir "build\win-86_v140"
cd "build\win-86_v140"

call cmake -G "Visual Studio 14 2015" -DCMAKE_GENERATOR_PLATFORM=Win32 -DCMAKE_GENERATOR_TOOLSET=v140 ..\..\..
call %msbuild% vanillapdf.sln /t:vanillapdf /p:Configuration=Release

cd ..\..
:: ------------- win-86_v140 -------------

:: ------------- win-64_v140 -------------
mkdir "build\win-64_v140"
cd "build\win-64_v140"

call cmake -G "Visual Studio 14 2015" -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_GENERATOR_TOOLSET=v140 ..\..\..
call %msbuild% vanillapdf.sln /t:vanillapdf /p:Configuration=Release

cd ..\..
:: ------------- win-86_v140 -------------

:: ------------- linux-x86 -------------
mkdir "build\linux-x86"
cd "build\linux-x86"

::call cmake -DCMAKE_GENERATOR_PLATFORM=win-64 -DCMAKE_GENERATOR_TOOLSET=v140 ../../..
::call %msbuild% vanillapdf.sln /t:vanillapdf /p:Configuration=Release

cd ..\..
:: ------------- linux-x86 -------------

:: ------------- NUGET_FINAL -------------
::rm vanillapdf.nupkg
nuget pack vanillapdf.nuspec
:: ------------- NUGET_FINAL -------------
