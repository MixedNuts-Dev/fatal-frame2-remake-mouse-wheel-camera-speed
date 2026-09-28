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
set "COMMON=%ROOT%mod-loader\common"
if not exist "%COMMON%\mixednuts\log.hpp" (echo [NG] mod-loader submodule is missing. Run: git submodule update --init & exit /b 1)
if not exist "%OUT%\Mods\wheelspeed" mkdir "%OUT%\Mods\wheelspeed"
if not exist "%OBJ%" mkdir "%OBJ%"

echo === loader (version.dll) ===
cl /nologo /LD /O2 /EHsc /MT /W3 /std:c++17 /utf-8 /DNDEBUG /I"%COMMON%" /Fo"%OBJ%\l_" /Fe"%OUT%\version.dll" "%ROOT%src\loader\version.cpp" /link /DEF:"%ROOT%src\loader\version.def" /OPT:REF /OPT:ICF
if errorlevel 1 exit /b 1

echo === payload (wheelspeed.dll) ===
cl /nologo /LD /O2 /EHsc /MT /W3 /std:c++17 /utf-8 /DNDEBUG /I"%COMMON%" /Fo"%OBJ%\p_" /Fe"%OUT%\Mods\wheelspeed\wheelspeed.dll" "%ROOT%src\payload\wheelspeed.cpp" /link /OPT:REF /OPT:ICF
if errorlevel 1 exit /b 1

echo === copying package files ===
copy /y "%ROOT%package\Mods\wheelspeed\wheelspeed.ini" "%OUT%\Mods\wheelspeed\" >nul
copy /y "%ROOT%package\Mods\wheelspeed\README.md" "%OUT%\Mods\wheelspeed\" >nul
copy /y "%ROOT%LICENSE" "%OUT%\Mods\wheelspeed\LICENSE.txt" >nul

rem import library / export file are build by-products
if exist "%OUT%\version.lib" del "%OUT%\version.lib"
if exist "%OUT%\version.exp" del "%OUT%\version.exp"
if exist "%OUT%\Mods\wheelspeed\wheelspeed.lib" del "%OUT%\Mods\wheelspeed\wheelspeed.lib"
if exist "%OUT%\Mods\wheelspeed\wheelspeed.exp" del "%OUT%\Mods\wheelspeed\wheelspeed.exp"

echo.
echo === done: %OUT% ===
dir /b /s "%OUT%"
endlocal
