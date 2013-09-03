@echo off

set VisualStudioVersion=2010

if [%1]==[] (
	goto :Help
)
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
	)
	if "%%A"=="-all" (
		call :SetupEnvironment
		call :BuildDependencies
		call :BuildProject
	)
)
call :Exit
goto :eof

:Help
	echo.
	echo The following arguments are available:
	echo.
	echo -help              Display this message
	echo -dependencies      Build dependencies
	echo -project           Build project
	echo -all               Build dependencies and project
	echo.
	goto :Exit
	
:SetupEnvironment
	echo --- Setting up environment
	echo.
	if "%VisualStudioVersion%" == "2008" (
		call "%VS90COMNTOOLS%vsvars32.bat"
		set "MSBUILD=%FrameworkDir%\%Framework35Version%\msbuild.exe"
	)
	if "%VisualStudioVersion%" == "2010" (
		call "%VS100COMNTOOLS%vsvars32.bat"
		set "MSBUILD=%FrameworkDir32%%FrameworkVersion32%\msbuild.exe"
	)
	echo.
	goto :eof

:BuildDependencies
	echo --- Building dependencies
	echo.
	%MSBUILD% /nologo /p:Configuration=Debug "ExLibrisGL.Dependencies.sln"
	%MSBUILD% /nologo /p:Configuration=Release "ExLibrisGL.Dependencies.sln"
	echo.
	echo --- Completed building the dependencies.
	goto :eof
	
:BuildProject
	echo --- Building project
	echo.
	%MSBUILD% /nologo /p:Configuration=Debug "ExLibrisGL.sln"
	%MSBUILD% /nologo /p:Configuration=Release "ExLibrisGL.sln"
	echo.
	echo --- Completed building the project.
	goto :eof

:Exit
	echo --- Done.
	goto :eof