@echo off

echo %PATH% | find "Common7" >nul
if errorlevel 1 (
	call inject_msvc
)
if "%1" == "--unity" (
	cl /DUNITY /EHs /Iinclude /nologo /std:c++20 src/main.cpp /Feaoc.exe /DEBUG:FULL /Zi
) else (
	cl /EHs /Iinclude /nologo /std:c++20 src/*.cpp /Feaoc.exe /DEBUG:FULL /Zi
)
del *.obj
