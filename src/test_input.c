#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// define a simple data strucure
typedef struct _Coord {
    int x, y;
} Coord;
typedef struct _Data {
    Coord pos;
    double val;
} Data;

// calc pixels from CCD raw data to the real pixel
// definition need checking
int calc_pixel(Data *CCD, Data *pixel) {

    return 0;
}

typedef struct _Range {
    Coord r1; // top-left point
    Coord r2; // down-right point
} Range;

double Data_at(Data *data, int x, int y) {
    uint32_t idx = 0;
    return data[idx].val;
}

// aux find column func
uint32_t find_col(Data *data) {
    uint32_t col = 0;
    while ((data[col].pos.x) != 1) {
        col++;
    }
    return col;
}

double black_uniformity(Data *data, Range r) {
    double min = 10, max = 0, tmp;
    uint32_t col = find_col(data);
    printf("col = %" PRIu32 "\n", col);
    for (uint32_t x = r.r1.x; x <= r.r2.x; x++) {
        for (uint32_t y = r.r1.y; y <= r.r2.y; y++) {
            tmp = data[x + (col*y)].val;
            if (min > tmp) {
                min = tmp;
            }
            if (max < tmp) {
                max = tmp;
            }
        }
    }
    return min/max;
}

// test printer
void Data_print(Data *self, uint32_t num_of_data) {
    for(uint32_t idx = 0; idx < num_of_data; idx++) {
        printf("Data (x, y) = val -> (%d, %d) = %.2lf\n",self[idx].pos.x, self[idx].pos.y, self[idx].val);
    }
}

// test input output
int main(int argc, char * argv[]) {

    const size_t data_size = 500000;
    Data *a = (Data *)calloc(sizeof(Data), data_size);
    
    Data tmp;
    uint32_t counter = 0;

    while(fscanf(stdin,"%d,%d,%lf", &tmp.pos.x, &tmp.pos.y, &tmp.val ) != EOF){
        a[counter].pos.x = tmp.pos.x;
        a[counter].pos.y = tmp.pos.y;
        a[counter].val = tmp.val;
        counter++;
    }
    // Data_print(a, counter);
    Range r = {{0,0},{992,489}};
    double u = black_uniformity(a, r);
    printf("U = %.2f\n", u);
    free(a);
    return 0;
}