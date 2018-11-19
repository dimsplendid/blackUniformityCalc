#include "BlackMura.h"
#include <string.h>

int main(int argc, char const *argv[]) {
    // check command input
    if (argc < 6) {
        printf("We need csv raw data and edge distance.\n");
        exit(EXIT_FAILURE);
    }
    Coord x1y1, x2y2;
    x1y1.x = strtoul(argv[2], NULL, 10);
    x1y1.y = strtoul(argv[3], NULL, 10);
    x2y2.x = strtoul(argv[4], NULL, 10);
    x2y2.y = strtoul(argv[5], NULL, 10);

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
    Range r = { x1y1 , {pixel->range.x - x2y2.x, pixel->range.y - x2y2.y}};
    double U = black_uniformity(pixel, r);
    printf("The Black Uniformity of %s is %.2f%%\n", CCD->name, U * 100);

    // print pixel data to check
    Data_print(pixel);
    // release memory
    Data_free(CCD);
    Data_free(pixel);

    return 0;
}
