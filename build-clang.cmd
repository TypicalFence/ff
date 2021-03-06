@ECHO OFF
::
:: CLI reminder
:: -O3
::
:: Don't forget setargv_*.obj
::
set t="\"%date% %time%\""
clang src\*.c src\executables\*.c src\etc\*.c src\images\*.c src\documents\*.c src\audio\*.c src\archives\*.c src\vdisk\*.c %1 %2 %3 %4 %5 -std=c99 -fno-stack-protector -fshort-enums -fstrict-enums -fpack-struct=1 -off.exe -DTIMESTAMP=%t%