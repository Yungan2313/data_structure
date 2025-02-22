@echo off
setlocal enabledelayedexpansion

set size=1000
set runs=100
set resultfile1=run%runs%_1.txt
rem set resultfile2=result%size%_old_run_2.txt

gcc -O2 411410010_hw3.c -o run

type nul > %resultfile1%
rem type nul > %resultfile2%

for /L %%i in (1, 1, %runs%) do (
    set /a j=%%i
    set /a k=j-1

    set /p output=<set\graph!j!.txt
    echo !output!>>%resultfile1%

    rem set /p output=<old_run < sample5.txt
    rem echo !output!>>%resultfile2%

    echo Run %%i completed.
)

echo All runs completed. Results stored in result.txt.