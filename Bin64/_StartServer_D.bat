@ECHO OFF

start Master_x64_D.exe
TIMEOUT /T 2 /NOBREAK > NUL

start Channel_x64_D.exe
TIMEOUT /T 2 /NOBREAK > NUL

start Gateway_x64_D.exe
TIMEOUT /T 2 /NOBREAK > NUL