@echo off
setlocal enabledelayedexpansion

set VisualStudioVersion=2010

if [%1]==[] (
	call :DoAll
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
) else (
	for %%A in (%*) do (
		if "%%A"=="-help" (
			call :Help
		)
		if "%%A"=="-dependencies" (
			call :DoDependencies
			if not %ERRORLEVEL%==0 (
				exit /B 1
			)
		)
		if "%%A"=="-project" (
			call :DoProject
			if not %ERRORLEVEL%==0 (
				exit /B 1
			)
		)
		if "%%A"=="-tests" (
			call :DoTests
			if not %ERRORLEVEL%==0 (
				exit /B 1
			)
		)
		if "%%A"=="-testsfast" (
			call :DoFastTests
			if not %ERRORLEVEL%==0 (
				exit /B 1
			)
		)
		if "%%A"=="-all" (
			call :DoAll
			if not %ERRORLEVEL%==0 (
				exit /B 1
			)
		)
	)
)
if not %ERRORLEVEL%==0 (
	call :BuildFailed
	exit /B 1
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
	goto :eof

:BuildDependencies
	echo --- Building dependencies
	echo.
	%MSBUILD% "ExLibrisGL.Dependencies.sln" /nologo /p:Configuration=Debug
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		exit /B 1
	)
	%MSBUILD% "ExLibrisGL.Dependencies.sln" /nologo /p:Configuration=Release
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		exit /B 1
	)
	echo.
	echo --- Completed building the dependencies.
	goto :eof
	
:BuildProject
	echo --- Building project
	echo.
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Debug /t:ExLibrisGL;Build /t:Example;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		exit /B 1
	)
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Release /t:ExLibrisGL;Build /t:Example;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build project.
		exit /B 1
	)
	echo.
	echo --- Completed building the project.
	goto :eof
	
:BuildTests
	echo --- Building tests
	echo.
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Debug /t:ExLibris_Test;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build tests.
		exit /B 1
	)
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Release /t:ExLibris_Test;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build tests.
		exit /B 1
	)
	echo.
	echo --- Completed building the tests.
	goto :eof
	
:RunTests
	echo --- Running tests (debug)
	echo.
	pushd Build
		call "ExLibris.TestDebug.exe"
		echo.
		echo --- Running tests (release)
		echo.
		call "ExLibris.TestRelease.exe"
	popd
	goto :eof

:DoDependencies
	call :SetupEnvironment
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildDependencies
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	goto :eof
	
:DoProject
	call :SetupEnvironment
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildProject
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildTests
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :RunTests
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	goto :eof
	
:DoTests
	call :SetupEnvironment
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildTests
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :RunTests
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	goto :eof
	
:DoFastTests
	call :SetupEnvironment
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	echo --- Building tests
	echo.
	%MSBUILD% "ExLibrisGL.sln" /nologo /p:Configuration=Debug /t:ExLibris_Test;Build
	if not %ERRORLEVEL% == 0 (
		set FAILEDMESSAGE=Failed to build tests.
		exit /B 1
	)
	echo.
	echo --- Running tests
	echo.
	pushd Build
	call "ExLibris.TestDebug.exe"
	popd
	echo.
	goto :eof

:DoAll
	echo --- All
	echo.
	call :SetupEnvironment
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildDependencies
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildProject
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :BuildTests
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	call :RunTests
	if not %ERRORLEVEL%==0 (
		exit /B 1
	)
	echo.
	goto :eof

:Exit
	echo --- Done
	endlocal
	goto :eof
	
:BuildFailed
	echo.
	echo --- Build failed
	echo.
	echo %FAILEDMESSAGE%
	exit /B 1