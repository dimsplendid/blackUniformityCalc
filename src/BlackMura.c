#include "BlackMura.h"

#include <string.h>

// print the data as csv
void Data_print(Data *self) {
    FILE *f = fopen(self->name, "w");
    for(uint32_t x = 0; x < self->range.x; x++) {
        for(uint32_t y = 0; y < self->range.y; y++){
            Datus *datus = &self->data[x * self->range.y + y];
            fprintf(f,"%" PRIu32 ", %" PRIu32 ", %.3f\n", \
                datus->pos.x, datus->pos.y, datus->val);
        }
    }
    fclose(f);
}

uint32_t find_yrange(Datus *data) {
    uint32_t yrange = 0;
    while ((data[yrange].pos.x) != 1){
        yrange++;
    }
    // printf("yrange = %" PRIu32 "\n", yrange);
    return yrange;
}

int Data_init(Data **self, const char *file_name) {
    if(((*self) = malloc(sizeof(Data))) == NULL) {
        exit(EXIT_FAILURE);
    }
    Datus *data = NULL;
    if((data=(Datus *)calloc(sizeof(Datus), MAXDATASIZE)) == NULL) {
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(file_name, "r");

    uint32_t counter = 0;
    while(fscanf(f, "%" PRIu32 ",%" PRIu32 ",%lf", \
           &data[counter].pos.x, &data[counter].pos.y, &data[counter].val) != EOF) {
        counter++;
    }
    fclose(f);
    (*self)->data = data;
    strcpy((*self)->name, file_name);
    (*self)->range.y = find_yrange(data);
    (*self)->range.x = counter / (*self)->range.y;
    // printf("range x:%" PRIu32 " y: %" PRIu32 "\n", (*self)->range.x, (*self)->range.y);
    return 0;
}
int Data_calloc(Data **self) {
    if(((*self) = malloc(sizeof(Data))) == NULL) {
        exit(EXIT_FAILURE);
    }
    Datus *data = NULL;
    if((data=(Datus *)calloc(sizeof(Datus), MAXDATASIZE)) == NULL) {
        exit(EXIT_FAILURE);
    }
    (*self)->data = data;
    return 0;
}

int Data_free(Data *self) {
    if (self->data != NULL){
        free(self->data);
    }
    free(self);
    return 0;
}

int CCD2AnalysisBox(Data *CCD, Data *AnalysisBox) {
    uint32_t avg_block_length = 8;
    uint32_t avg_block_size = avg_block_length * avg_block_length;
    
    // Init AnalysisBox parameters
    AnalysisBox->range.x = CCD->range.x - avg_block_length + 1;
    AnalysisBox->range.y = CCD->range.y - avg_block_length + 1;
    snprintf(AnalysisBox->name, 20, "%s_", CCD->name);

    for(uint32_t x = 0; x < AnalysisBox->range.x; x++) {
        for(uint32_t y = 0; y < AnalysisBox->range.y; y++) {
            // calc block avg
            double avg_tmp = 0.0;
            for(uint32_t x_block = 0; x_block < avg_block_length; x_block++) {
                for(uint32_t y_block = 0; y_block < avg_block_length; y_block++) {
                    avg_tmp += CCD->data[((x + x_block) * CCD->range.y) + (y + y_block)].val / avg_block_size;
                }
            }
            /// copy data to AnalysisBox array
            AnalysisBox->data[x * AnalysisBox->range.y + y].pos.x = x;
            AnalysisBox->data[x * AnalysisBox->range.y + y].pos.y = y;
            AnalysisBox->data[x * AnalysisBox->range.y + y].val = avg_tmp;
        }
    }
    return 0;
}

double black_uniformity(Data *pixel, Range r, Datus minMax[6]) {
    double min = 10, max = 1e-6, tmp = 0;
    for(uint32_t x = r.x1y1.x; x < r.x2y2.x; x++) {
        for(uint32_t y = r.x1y1.y; y < r.x2y2.y; y++) {
            tmp = pixel->data[x * pixel->range.y + y].val;
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
