:: batch.cmd
:: 
:: copyright (c) 2018
::  Wei-Hsiang Tseng
::  all right reserved
:: Tools for batch calculation all csv files

@echo off
set x1=100
set y1=50
set x2=100
set y2=50

set avg_box_length=7

del ouput\out.csv /F /Q
:: items name
echo ID,BlackU,BlackU_cut,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,comment, block_len=%avg_box_length% >> output\out.csv
for %%f in (*.csv) do (
    echo %%~nf
    BlackMura_win "%%~nf.csv" %x1% %y1% %x2% %y2% %avg_box_length% >> output\out.csv
)
