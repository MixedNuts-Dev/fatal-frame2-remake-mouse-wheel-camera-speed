@echo off
rem FATAL FRAME II: Crimson Butterfly REMAKE - Mouse Wheel Camera Speed
rem Builds dist\ containing the files users drop into the game root.
setlocal
set "VS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VS%" (echo [NG] vcvars64.bat not found: %VS% & exit /b 1)
call "%VS%" >nul
if errorlevel 1 exit /b 1

set "ROOT=%~dp0"
set "OUT=%ROOT%dist"
set "OBJ=%ROOT%obj"
set "LOADER=%ROOT%mod-loader"
rem set MIXEDNUTS_LOADER to build against a local checkout of mod-loader instead
if defined MIXEDNUTS_LOADER set "LOADER=%MIXEDNUTS_LOADER%"
if not exist "%LOADER%\api\mixednuts\plugin.h" (echo [NG] mod-loader submodule is missing or old. Run: git submodule update --init & exit /b 1)
rem a MixedNuts Mod Loader plugin: MixedNuts\Mods\wheelspeed\wheelspeed.dll
set "DST=%OUT%\MixedNuts\Mods\wheelspeed"
if exist "%OUT%" rmdir /s /q "%OUT%"
mkdir "%DST%"
if not exist "%OBJ%" mkdir "%OBJ%"

echo === plugin (wheelspeed.dll) ===
cl /nologo /LD /O2 /EHsc /MT /W3 /std:c++17 /utf-8 /DNDEBUG /I"%LOADER%\common" /I"%LOADER%\api" /Fo"%OBJ%\p_" /Fe"%DST%\wheelspeed.dll" "%ROOT%src\wheelspeed.cpp" /link /OPT:REF /OPT:ICF
if errorlevel 1 exit /b 1

echo === copying package files ===
copy /y "%ROOT%package\Mods\wheelspeed\wheelspeed.ini" "%DST%\" >nul
copy /y "%ROOT%package\Mods\wheelspeed\README.md" "%DST%\" >nul
copy /y "%ROOT%LICENSE" "%DST%\LICENSE.txt" >nul

rem import library / export file are build by-products
if exist "%DST%\wheelspeed.lib" del "%DST%\wheelspeed.lib"
if exist "%DST%\wheelspeed.exp" del "%DST%\wheelspeed.exp"

echo.
echo === done: %OUT% ===
dir /b /s "%OUT%"
endlocal
