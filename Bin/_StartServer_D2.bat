@ECHO OFF

start Master_x64_D.exe
TIMEOUT /T 2 /NOBREAK > NUL

start Channel_x64_D.exe -f servers_chns1.ini
TIMEOUT /T 2 /NOBREAK > NUL

start Channel_x64_D.exe -f servers_chns2.ini
TIMEOUT /T 2 /NOBREAK > NUL

start Gateway_x64_D.exe -f servers_gtws1.ini
TIMEOUT /T 2 /NOBREAK > NUL

start Gateway_x64_D.exe -f servers_gtws2.ini
TIMEOUT /T 2 /NOBREAK > NUL