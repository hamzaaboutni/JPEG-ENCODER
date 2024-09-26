#include <stdio.h>
#include <stdlib.h>
#include "../include/SDD.h"
#include "../include/complement.h"
#include "../include/zz_q.h"
#include "../include/qtables.h"

int **transformer_1D_2D(int *array){
    int **block=malloc(8*sizeof(int *));
    for(int i=0; i<8; i++){
        block[i]=malloc(8*sizeof(int));
        for(int j=0; j<8; j++){
            block[i][j]=array[i*8+j];
        }
    }
    return block;
}

void zzq_liste_block(YCbCrBlock* liste_blocks, int total_blocks) {
    for (int i = 0; i < total_blocks; i++) {
        int *zzq = malloc(64 * sizeof(int));
        convertir_en_zigzag(liste_blocks[i].block, zzq);
        if (liste_blocks[i].type == 0) {
            quantification(zzq, quantification_table_Y);
        } else {
            quantification(zzq, quantification_table_CbCr);
        }
        int** block_zzq = transformer_1D_2D(zzq);
        liste_blocks[i].block = block_zzq;
        free(zzq);
    }
}

YCbCrBlock *convert_blocks_to_zigzag(const YCbCrBlock *liste_blocks, int total_blocks) {
    YCbCrBlock *output_blocks = malloc(total_blocks * sizeof(YCbCrBlock)); // Allocate output blocks

    for (int i = 0; i < total_blocks; i++) {
        int *zigzag_array = malloc(64 * sizeof(int));
        convertir_en_zigzag(liste_blocks[i].block, zigzag_array);
        output_blocks[i].block = transformer_1D_2D(zigzag_array);
        output_blocks[i].type = liste_blocks[i].type; // Copy type from input to output
        free(zigzag_array); // Free the temporary 1D array
    }

    return output_blocks;
}

void write_to_file(const char* filename, const char* bitstream) {
    FILE *file = fopen(filename, "w");
    fputs(bitstream, file);
    fclose(file);
}
