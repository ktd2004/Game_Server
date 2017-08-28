@ECHO OFF

SET VARS="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
SET MSDEV=devenv
SET SERVERLIST=build_server.txt

:: Build options
:: build	// default
:: clean
:: rebuild
SET OPT=""

:: Build Composition
:: debug	// default
:: release
SET COMPOSITION=""

SET SKIP_PAUSE="0"

:: Log Date
SET LOGDATE=%DATE:-=%

:: Log Time
SET TEMPTIME=%TIME::=%
SET LOGTIME=%TEMPTIME:~0,6%

:: Build Log
SET BUILDLOG=Build_server_x64_%LOGDATE%_%LOGTIME%.log

:: HELP
IF "%1" == "/?" (
	GOTO HELP
)

:: File Check
IF NOT EXIST %SERVERLIST% (
	ECHO Error : Not found file - %SERVERLIST%
	GOTO QUIT
)

IF "%1" == "" (
	ECHO Set default build option : /Build
	SET OPT=/Build
	GOTO SETCOMPOSITION
)

IF "%1" == "-build" (
	SET OPT=/Build
	GOTO SETCOMPOSITION
)
IF "%1" == "-clean" (
	SET OPT=/Clean
	GOTO SETCOMPOSITION
)
IF "%1" == "-rebuild" (
	SET OPT=/Rebuild
	GOTO SETCOMPOSITION
) ELSE (
	ECHO Error : Invalid Build Option
	GOTO QUIT
)

:SETCOMPOSITION
IF "%2" == "" (
	ECHO Set default composition : Debug
	SET COMPOSITION=Debug
	GOTO RUN
)

IF "%2" == "-debug" (
	SET COMPOSITION=Debug
	GOTO SETSKIPPUASE
)
IF "%2" == "-release" (
	SET COMPOSITION=Release
	GOTO SETSKIPPUASE
)
IF "%2" == "-skip_pause" (
	SET SKIP_PAUSE="1"
	SET COMPOSITION=Debug
	GOTO RUN
) ELSE (
	ECHO Error : Invalid Composition Option
	GOTO QUIT
)

:SETSKIPPUASE
IF "%3" == "" (
	GOTO RUN
)

IF "%3" == "-skip_pause" (
	SET SKIP_PAUSE="1"
	GOTO RUN
) ELSE (
	GOTO RUN
)

:HELP
ECHO Usage : build_server_vc11.bat [-option [-composition]]
ECHO         -option      : -build, -clean, -rebuild
ECHO         -composition : -debug, -release
ECHO         -skip_pause
GOTO QUIT

:RUN
ECHO [[Current Option : %OPT%, %COMPOSITION%]]
IF "%INCLUDE%" == "" (
	ECHO Run Visual Studio Environment Script
	@CALL %VARS%
)

FOR /f %%S IN (%SERVERLIST%) DO (
	ECHO %OPT% - %%S
	@CALL %MSDEV% ..\RPC.NET.ver.0.1.sln %OPT% "%COMPOSITION%|x64" /Project %%S /ProjectConfig "%COMPOSITION%|x64" /Out %BUILDLOG%
)

ECHO [[Build Finished]]
GOTO QUIT

:QUIT
IF "%SKIP_PAUSE%" == "0" (
	PAUSE
)
