#include "BlackMura.h"

#include <string.h>

void Data_print(Data *self) {
    for(uint32_t i = 0; i < self->rows * self->cols; i++) {
        Datus *datus = &self->data[i];
        printf("(%" PRIu32 ", %" PRIu32 ") = %.3f", \
                datus->pos.x, datus->pos.y, datus->val);
    }
}

uint32_t find_cols(Datus *data) {
    uint32_t cols = 0;
    while ((data[cols].pos.x) != 1){
        cols++;
    }
    return cols;
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
    (*self)->cols = find_cols(data);
    (*self)->rows = counter / (*self)->cols;
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
    (*self)->cols = 0;
    (*self)->rows = 0;
    return 0;
}

int Data_free(Data *self) {
    if (self->data != NULL){
        free(self->data);
    }
    free(self);
    return 0;
}

int CCD2pixel(Data *CCD, Data *pixel) {
    uint32_t avg_block_length = 8;
    uint32_t avg_block_size = avg_block_length * avg_block_length;
    
    // Init pixel parameters
    pixel->rows = CCD->rows - avg_block_length + 1;
    pixel->cols = CCD->cols - avg_block_length + 1;
    strcpy(pixel->name, CCD->name);

    for(uint32_t x = 0; x < pixel->rows; x++) {
        for(uint32_t y = 0; y < pixel->rows; y++) {
            // calc block avg
            double avg_tmp = 0.0;
            for(uint32_t x_block = 0; x_block < avg_block_length; x_block++) {
                for(uint32_t y_block = 0; y_block < avg_block_length; y_block++) {
                    avg_tmp += CCD->data[x + x_block + (y + y_block) * CCD->cols].val / avg_block_size;
                }
            }
            /// copy data to pixel array
            pixel->data[x + pixel->cols * y].pos.x = x;
            pixel->data[x + pixel->cols * y].pos.y = y;
            pixel->data[x + pixel->cols * y].val = avg_tmp;
        }
    }
    return 0;
}

double black_uniformity(Data *pixel, Range r) {
    double min = 10, max = 1e-6, tmp = 0;
    for(uint32_t x = r.x1y1.x; x <= r.x2y2.x; x++) {
        for(uint32_t y = r.x1y1.y; y <= r.x2y2.y; y++) {
            tmp = pixel->data[x + pixel->cols * y].val;
            if (min > tmp)
                min = tmp;
            if (max < tmp)
                max = tmp;
        }
    }
    return min/max;
}
