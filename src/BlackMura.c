#include "BlackMura.h"

#include <string.h>

void Data_print(Data *self) {
    for(uint32_t i = 0; i < self->rows * self->cols; i++) {
        Datus *datus = &self->data[i];
        printf("(%" PRIu32 ", %" PRIu32 ") = %.3f", \
                datus->pos.x, datus->pos.y, datus->val);
    }
}

/* read aux function */

// return cols of the given data
// ps: Must in the right data struct
uint32_t find_cols(Datus *data) {
    uint32_t cols = 0;
    while ((data[cols].pos.x) != 1){
        cols++;
    }
    return cols;
}

int Data_init(Data **self, char *file_name) {
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
    (*self)->rows = counter / cols;

    return 0;
}
int Data_calloc(Data **self) {
    Datus *data = NULL;
    if((data=(Datus *)calloc(sizeof(Datus), MAXDATASIZE)) == NULL) {
        exit(EXIT_FAILURE);
    }
    (*self)->data = data;
    (*self)->cols = 0;
    (*self)->rows = 0;
}

/* Aux Func of Black Uniformity */
int CCD2pixel(Data *CCD, Data *pixel);
double black_uniformity(Data *data, Range r);
