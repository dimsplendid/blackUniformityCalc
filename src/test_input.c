#include <stdio.h>
#include <stdlib.h>

// define a simple data strucure
typedef struct _Coord {
    int x, y;
} Coord;
typedef struct _Data {
    Coord pos;
    double val;
} Data;
// test input output
int main(int argc, char * argv[]) {
    const size_t data_size = 500000;
    Data *a = (Data *)calloc(sizeof(Data), data_size);
    fscanf(stdin,"%d, %d, %lf", &a[0].pos.x, &a[0].pos.y, &a[0].val);
    printf("Data (x, y) = val -> (%d, %d) = %.2lf\n",a[0].pos.x, a[0].pos.y, a[0].val);
    free(a);
    return 0;
}