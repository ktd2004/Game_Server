@ECHO OFF

cmd.exe /c build_lib_x64.bat -rebuild -skip_pause
cmd.exe /c build_server_x64.bat -rebuild -skip_pause

pause
