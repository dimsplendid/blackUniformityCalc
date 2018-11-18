#include "BlackMura.h"
#include <string.h>

int main(int argc, char const *argv[]) {
    // check command input
    if (argc < 3) {
        printf("We need csv raw data and edge distance.\n");
        exit(EXIT_FAILURE);
    }
    uint32_t edge_dist = 0;
    edge_dist = strtoul(argv[2], NULL, 10);
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
    Range r = {{edge_dist, edge_dist}, {pixel->range.x - edge_dist, pixel->range.y - edge_dist}};
    double U = black_uniformity(pixel, r);
    printf("The Black Uniformity of %s with edge distance %" PRIu32 " is %.4f%%\n", pixel->name, edge_dist, U * 100);

    // print pixel data to check
    Data_print(pixel);
    // release memory
    Data_free(CCD);
    Data_free(pixel);

    return 0;
}
