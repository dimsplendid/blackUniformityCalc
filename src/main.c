/**
 * main.c
 * 
 * copyright (c) 2018
 *  Wei-Hsiang Tseng
 *  all right reserved
 * 
 */
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
    Data *CCD, *AnalysisBox;
    Data_init(&CCD, file_name);
    
    Data_calloc(&AnalysisBox);
    // conver CCD raw data to AnalysisBox by calculating block average
    // printf("conver CCD raw to AnalysisBox...\n");

    CCD2AnalysisBox(CCD, AnalysisBox);
    
    // Calc Black Uniformity within range r
    // printf("cal Black U...\n");
    Range r0 = {{0, 0}, {AnalysisBox->range.x, AnalysisBox->range.y}};
    Range r = { x1y1 , {AnalysisBox->range.x - x2y2.x, AnalysisBox->range.y - x2y2.y}};
    Datus *minMax = calloc(6, sizeof(Datus));
    double U0 = black_uniformity(AnalysisBox, r0, minMax);
    double U = black_uniformity(AnalysisBox, r, minMax);

    printf("%s,%.2f%%,%.2f%%", CCD->name, U0 * 100, U * 100);
    for (uint32_t i = 0; i < 6; i++) {
        printf("%" PRIu32 ",", minMax[6 - 1 - i].pos.x);
        printf("%" PRIu32 ",", minMax[6 - 1 - i].pos.y);
    }
    printf("descending order\n");

    // print AnalysisBox data to check
    const char output_name[80];
    #ifdef WINDOWS
    snprintf((char*)output_name, 80, "output\\%s",CCD->name);
    #endif
    #ifdef LINUX
    snprintf((char*)output_name, 80, "output/%s",CCD->name);
    #endif
    Data_print(AnalysisBox, output_name);

    // release memory
    Data_free(CCD);
    Data_free(AnalysisBox);
    free(minMax);

    return 0;
}
