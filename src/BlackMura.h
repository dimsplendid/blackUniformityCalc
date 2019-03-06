/**
 * BlackMura.h
 * 
 * copyright (c) 2018
 *  Wei-Hsiang Tseng
 *  all right reserved
 * 
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Constant */
#define MAXDATASIZE 1000000

/* Data structures */

// position, using x, y coordinate
typedef struct _Coord {
    uint32_t x, y; // x, y belong to N
} Coord;

// each val at (x, y)
typedef struct _Datus {
    Coord pos;
    double val;
} Datus;

// input data
typedef struct _Data {
    char name[20];
    Coord range;
    Datus *data; // index (x, y) -> data[x*yrange + y]
} Data;

// Data select
typedef struct _Range {
    Coord x1y1; // top-left point of the matrix
    Coord x2y2; // down-right point of the matrix
} Range;

void Data_print(Data *self, const char *file_name);

/* read input file aux function */

// return y-range of the given data
// ps: Must in the right data struct
uint32_t find_cols(Datus *data);

// Data construct
int Data_init(Data **self, const char file_name[80]);
int Data_calloc(Data **self);
// Data destruct
int Data_free(Data *self);

/* Aux Func of Black Uniformity */
// convert CCD raw data to analysis box luminance
int CCD2AnalysisBox(Data *CCD, Data *AnalysisBox, uint32_t box_size);
// Calculate black uniformity: U% = I_min / I_max
// minMax[0..2] -> Three smallest data
// minMax[3..5] -> Three largest data
double black_uniformity(Data *data, Range r, Datus minMax[6]);
