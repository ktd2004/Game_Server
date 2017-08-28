@ECHO OFF

SET VARS="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
SET MSDEV="devenv"
SET CP="xcopy"
SET RM="del"
SET OPT=""

:: Log Date
SET LOGDATE=%DATE:-=%

:: Log Time
SET TEMPTIME=%TIME::=%
SET LOGTIME=%TEMPTIME:~0,6%

:: Build Log
SET BUILDLOG=Build_libex_%LOGDATE%_%LOGTIME%.log

@CALL %VARS%

IF "%1" == "" (
	ECHO Set default build option : /Build
	SET OPT=/Build
)
IF "%1" == "-build" (
	SET OPT=/Build
)
IF "%1" == "-clean" (
	SET OPT=/Clean
)


ECHO ++++CREDIS++++
@CALL %MSDEV% ../sdk/libex/credis-0.2.3/credis.sln %OPT% "Debug|Win32" /Project credis /ProjectConfig "Debug|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/credis-0.2.3/credis.sln %OPT% "Release|Win32" /Project credis /ProjectConfig "Release|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/credis-0.2.3/credis.sln %OPT% "Debug|x64" /Project credis /ProjectConfig "Debug|x64" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/credis-0.2.3/credis.sln %OPT% "Release|x64" /Project credis /ProjectConfig "Release|x64" /Out %BUILDLOG%

ECHO ++++Google TCMALLOC++++
@CALL %MSDEV% ../sdk/libex/gperftools-wip-issue-610/gperftools.sln %OPT% "Debug|Win32" /Project libtcmalloc_minimal /ProjectConfig "Debug|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/gperftools-wip-issue-610/gperftools.sln %OPT% "Release|Win32" /Project libtcmalloc_minimal /ProjectConfig "Release|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/gperftools-wip-issue-610/gperftools.sln %OPT% "Debug|x64" /Project libtcmalloc_minimal /ProjectConfig "Debug|x64" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/gperftools-wip-issue-610/gperftools.sln %OPT% "Release|x64" /Project libtcmalloc_minimal /ProjectConfig "Release|x64" /Out %BUILDLOG%

ECHO ++++JSONCPP++++
@CALL %MSDEV% ../sdk/libex/jsoncpp-src-0.5.0/makefiles/vs71/jsoncpp.sln %OPT% "Debug|Win32" /Project lib_json /ProjectConfig "Debug|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/jsoncpp-src-0.5.0/makefiles/vs71/jsoncpp.sln %OPT% "Release|Win32" /Project lib_json /ProjectConfig "Release|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/jsoncpp-src-0.5.0/makefiles/vs71/jsoncpp.sln %OPT% "Debug|x64" /Project lib_json /ProjectConfig "Debug|x64" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/jsoncpp-src-0.5.0/makefiles/vs71/jsoncpp.sln %OPT% "Release|x64" /Project lib_json /ProjectConfig "Release|x64" /Out %BUILDLOG%

ECHO ++++ZLIB++++
@CALL %MSDEV% ../sdk/libex/zlib123/projects/vs11/zlib.sln %OPT% "LIB Debug|Win32" /Project zlib /ProjectConfig "LIB Debug|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/zlib123/projects/vs11/zlib.sln %OPT% "LIB Release|Win32" /Project zlib /ProjectConfig "LIB Release|Win32" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/zlib123/projects/vs11/zlib.sln %OPT% "LIB Debug|x64" /Project zlib /ProjectConfig "LIB Debug|x64" /Out %BUILDLOG%
@CALL %MSDEV% ../sdk/libex/zlib123/projects/vs11/zlib.sln %OPT% "LIB Release|x64" /Project zlib /ProjectConfig "LIB Release|x64" /Out %BUILDLOG%

IF "%OPT%" == "/Build" (
@CALL %CP% ..\sdk\libex\gperftools-wip-issue-610\Win32\Debug\*.dll ..\Bin\ /y
@CALL %CP% ..\sdk\libex\gperftools-wip-issue-610\Win32\Release\*.dll ..\Bin\ /y
@CALL %CP% ..\sdk\libex\gperftools-wip-issue-610\x64\Debug\*.dll ..\Bin64\ /y
@CALL %CP% ..\sdk\libex\gperftools-wip-issue-610\x64\Release\*.dll ..\Bin64\ /y

@CALL %CP% ..\sdk\libex\maria-connector-c-2.0\lib\x86\*.dll ..\Bin\ /y
@CALL %CP% ..\sdk\libex\maria-connector-c-2.0\lib\x64\*.dll ..\Bin64\ /y

@CALL %CP% ..\sdk\libex\mysql-connector-c-6.1\lib\x86\*.dll ..\Bin\ /y
@CALL %CP% ..\sdk\libex\mysql-connector-c-6.1\lib\x64\*.dll ..\Bin64\ /y
)
IF "%OPT%" == "/Clean" (
@CALL %RM% ..\Bin\*.dll
@CALL %RM% ..\Bin64\*.dll
)