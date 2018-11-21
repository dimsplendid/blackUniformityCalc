/**
 * BlackMura.c
 * 
 * copyright (c) 2018
 *  Wei-Hsiang Tseng
 *  all right reserved
 * 
 */
 
#include "BlackMura.h"

#include <string.h>

// print the data as csv
void Data_print(Data *self, const char file_name[80]) {
    FILE *f = fopen(file_name, "w");
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

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(Datus arr[], int l, int m, int r) {
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    Datus *L = calloc(n1, sizeof(Datus));
    Datus *R = calloc(n2, sizeof(Datus)); 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) { 
        if (L[i].val <= R[j].val) {
            arr[k] = L[i]; 
            i++; 
        } 
        else {
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1) {
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    }
    free(L);
    free(R);
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(Datus arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for 
        // large l and r
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

double black_uniformity(Data *AnalysisBox, Range r, Datus minMax[6]) {
    // using Merge sort
    // copy array
    uint32_t yrange = r.x2y2.y - r.x1y1.y;
    uint32_t xrange = r.x2y2.x - r.x1y1.x;
    Datus *ActiveBox = calloc(xrange * yrange, sizeof(Datus));
    for (uint32_t x = 0; x < xrange; x++) {
        for(uint32_t y = 0; y < yrange; y++) {
            ActiveBox[x * yrange + y] = AnalysisBox->data[((r.x1y1.x + x) * AnalysisBox->range.y) + r.x1y1.y + y];
        }
    }
    mergeSort(ActiveBox, 0, xrange * yrange - 1);
    for (uint32_t i = 0; i < 3; i++) {
        minMax[i] = ActiveBox[i];
        minMax[6 - 1 - i] = ActiveBox[xrange * yrange - 1 - i];
    }

    free(ActiveBox);
    return minMax[0].val/minMax[5].val;
}
