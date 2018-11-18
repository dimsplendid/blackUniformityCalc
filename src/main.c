#include "BlackMura.h"
#include <string.h>

int main(int argc, char const *argv[]) {
    // check command input
    if (argc < 2) {
        printf("We need csv raw data and some options.\n");
        exit(EXIT_FAILURE);
    }

    // read file
    // printf("reading csv...\n");

    const char* file_name = strdup(argv[1]);
    Data *CCD, *pixel;
    Data_init(&CCD, file_name);
    Data_calloc(&pixel);
    // conver CCD raw data to pixel by calculating block average
    // printf("conver CCD raw to pixel...\n");

    CCD2pixel(CCD, pixel);
    
    // Calc Black Uniformity within range r
    // printf("cal Black U...\n");
    Range r = {{0, 0}, {pixel->cols, pixel->rows}};
    double U = black_uniformity(pixel, r);
    printf("The Black Uniformity of %s is %.4f%%\n", pixel->name, U * 100);

    // release memory
    Data_free(CCD);
    Data_free(pixel);

    return 0;
}
