:: batch.cmd
:: 
:: copyright (c) 2018
::  Wei-Hsiang Tseng
::  all right reserved
:: Tools for batch calculation all csv files

@echo off
set x1=%1
set y1=%2
set x2=%3
set y2=%4

del ouput\out.csv /F /Q
:: items name
echo ID,BlackU,BlackU_cut,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,comment >> output\out.csv
for %%f in (*.csv) do (
    echo %%~nf
    BlackMura_win "%%~nf.csv" %x1% %y1% %x2% %y2% >> output\out.csv
)
