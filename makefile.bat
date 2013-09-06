@echo off
setlocal enabledelayedexpansion

set VisualStudioVersion=2010

if [%1]==[] (
	call :All
) else (
	for %%A in (%*) do (
		if "%%A"=="-help" (
			call :Help
		)
		if "%%A"=="-dependencies" (
			call :SetupEnvironment
			call :BuildDependencies
		)
		if "%%A"=="-project" (
			call :SetupEnvironment
			call :BuildProject
			call :BuildTests
			call :RunTests
		)
		if "%%A"=="-tests" (
			call :SetupEnvironment
			call :BuildTests
			call :RunTests
		)
		if "%%A"=="-testsfast" (
			call :SetupEnvironment
			call :FastTests
		)
		if "%%A"=="-all" (
			call :All
		)
	)
)
call :Exit
exit /B 0

:Help
	echo.
	echo The following arguments are available:
	echo.
	echo -help              Display this message
	echo -dependencies      Build dependencies
	echo -project           Build project
	echo -tests             Build and run all tests
	echo -testsfast         Build and run only the debug version of tests
	echo -all               Build dependencies and project and run tests
	echo.
	goto :eof
	
:SetupEnvironment
	echo --- Setting up environment
	echo.
	if "%VisualStudioVersion%" == "2008" (
		call "%VS90COMNTOOLS%vsvars32.bat"
		set "MSBUILD=!FrameworkDir!\!Framework35Version!\msbuild.exe"
	)
	if "%VisualStudioVersion%" == "2010" (
		call "%VS100COMNTOOLS%vsvars32.bat"
		set "MSBUILD=!FrameworkDir32!!FrameworkVersion32!\msbuild.exe"
	)
	echo.
	goto :eof

:BuildDependencies
	echo --- Building dependencies
	echo.
	%MSBUILD% "ExLibrisGL.Dependencies.sln" /nologo /p:Configuration=Debug
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		call :BuildFailed
	)
	%MSBUILD% "ExLibrisGL.Dependencies.sln" /nologo /p:Configuration=Release
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		call :BuildFailed
	)
	echo.
	echo --- Completed building the dependencies.
	echo.
	goto :eof
	
:BuildProject
	echo --- Building project
	echo.
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Debug /t:ExLibrisGL;Build /t:Example;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		call :BuildFailed
	)
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Release /t:ExLibrisGL;Build /t:Example;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		call :BuildFailed
	)
	echo.
	echo --- Completed building the project.
	echo.
	goto :eof
	
:BuildTests
	echo --- Building tests
	echo.
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Debug /t:ExLibris_Test;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build tests.
		call :BuildFailed
	)
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Release /t:ExLibris_Test;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build tests.
		call :BuildFailed
	)
	echo.
	echo --- Completed building the tests.
	echo.
	goto :eof
	
:RunTests
	pushd Build
	echo --- Running tests (debug)
	echo.
	call "ExLibris.TestDebug.exe"
	echo.
	echo --- Running tests (release)
	echo.
	call "ExLibris.TestRelease.exe"
	echo.
	popd
	goto :eof
	
:FastTests
	echo --- Building tests
	echo.
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Debug /t:ExLibris_Test;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build tests.
		call :BuildFailed
	)
	echo.
	echo --- Running tests
	echo.
	pushd Build
	call "ExLibris.TestDebug.exe"
	echo.
	popd
	goto :eof
	
:All
	echo --- All
	echo.
	call :SetupEnvironment
	call :BuildDependencies
	call :BuildProject
	call :BuildTests
	call :RunTests
	echo.
	goto :eof
	
:BuildFailed
	echo.
	echo --- Build failed
	echo.
	echo %FAILEDMESSAGE%
	echo.
	goto :Exit
	
:Exit
	echo --- Done
	endlocal
	
	rem In order to actually exit the script, we generate a syntax error.
	rem Batch scripts are weird.
	call :ExitHelper 2> nul
	
:ExitHelper
	()
	exit /B