# Black Uniformity
> dimsplendid@gmail.com 
> [time=2018/11/18]
## Introduction
The simple black uniformity calculator 
## Definition
### Detector
CCD camera, which should have 64 pixels at the 2 mm length analysis square.  
The value of analysis square is the mean of 64 pixels, and analysis move step by step at 1 pixel.
So the size of total analysis squares, the analysis box, is (raw.xrange - 7) * ( raw.yrange - 7).
### Black Uniformity
At the given range of analysis boxes, 
U = I<sub>min</sub>/I<sub>max</sub>
## Usage
one raw data  
> the raw data formate must be  
> x<sub>1</sub>,y<sub>1</sub>,value<sub>1</sub>  
> ...  
> x<sub>N</sub>,y<sub>N</sub>,value<sub>N</sub>  

for Linux
```
BlackMura [path/to/raw.csv] [x1] [y1] [x2] [y2] [avg box length]
```
for Windows
```
BlackMura_win [path\to\raw.csv] [x1] [y1] [x2] [y2] [avg box length]
```
x1, y1, x2, y2 is the distance between "active box" and left, top, right, down of "analysis box".
And note that we should have a directory "output" in the same folder, the analysis box data would save in that with the same name. 

I also write a simple batch script for Windows at the directory ./tool/batch
```
batch [x1] [y1] [x2] [y2] %% or just change the value in the batch file
```
This would go through all the \*.csv file in the same directory, and save analysis box data and all output infomation as out.csv in the output directory.
