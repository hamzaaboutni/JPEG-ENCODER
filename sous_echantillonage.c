#include <stdio.h>
#include <stdlib.h>
#include "../include/SDD.h"
#include <stdbool.h>

int*** allouer_blocks(int num_blocks) {
    int ***blocks =malloc(num_blocks * sizeof(int **));
    for (int i = 0; i < num_blocks; i++) {
        blocks[i] =malloc(8* sizeof(int *));
        for (int j = 0; j < 8; j++) {
            blocks[i][j] =malloc(8* sizeof(int));
        }
    }
    return blocks;
}

void free_block(int **block) {
    for (int i = 0; i < 8; i++) {
        free(block[i]);
    }
    free(block);
}

void fill_block(int **matrix, int **block, int start_row, int start_col) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            block[i][j]= matrix[start_row +i][start_col + j];
        }
    }
}

int*** split_blocks(int **matrix,int width,int height, int *num_blocks) {
    int num_horizontal_blocks = width / 8;
    int num_vertical_blocks = height / 8;
    *num_blocks = num_horizontal_blocks* num_vertical_blocks;

    int ***blocks = allouer_blocks(*num_blocks);
    if (blocks == NULL) {
        return NULL;
    }
    int block_index = 0;
    for (int block_row = 0; block_row< num_vertical_blocks; block_row++) {
        for (int block_col = 0; block_col < num_horizontal_blocks; block_col++) {
            fill_block(matrix, blocks[block_index],block_row * 8, block_col * 8);
            block_index++;
        }
    }
    return blocks;
}



void free_YCbCrMCU(YCbCrMCU *mcu) {
    for (int i = 0; i <mcu->height; i++) {
        free(mcu->pixels[i]);
    }
    free(mcu->pixels);
    free(mcu);
}

int** creer_matrice(int width, int height) {
    int **matrix = malloc(height* sizeof(int *));
    for (int i = 0; i < height; i++) {
        matrix[i] = malloc(width* sizeof(int));
        }

    return matrix;
}

void free_matrix(int **matrix, int height) {
    for (int i = 0; i <height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill_subsampled_matrix(int **matrix,YCbCrPixel **pixels, int width, int height, int new_width, int new_height, char component) {
    int row = 0, col, idx, jdx, sum, count;
    while (row < new_height) {
        col = 0;
        while (col <new_width) {
            sum = 0;
            count = 0;
            idx = row * (height/new_height);
            while (idx < (row + 1)*(height/new_height) && idx < height) {
                jdx = col *(width / new_width);
                while (jdx < (col + 1) *(width / new_width) && jdx < width) {
                    if (component == 'Y') {
                        sum +=pixels[idx][jdx].y;
                    } else if (component == 'C') {
                        sum +=pixels[idx][jdx].cb;
                    } else if (component == 'R') {
                        sum +=pixels[idx][jdx].cr;
                    }
                    count++;
                    jdx++;
                }
                idx++;
            }
            matrix[row][col] =count? sum/count : 0;
            col++;
        }
        row++;
    }
}

MCU_subsampled *subsample(YCbCrMCU *input, int h2, int v2, int h3, int v3, bool is_pgm) {
    MCU_subsampled *result = malloc(sizeof(MCU_subsampled));
    int height = input->height, width = input->width;
    int **matrixY = creer_matrice(width, height);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            matrixY[row][col] = input->pixels[row][col].y;
        }
    }
    result->Y_blocks = split_blocks(matrixY, width, height, &result->num_Y_blocks);
    free_matrix(matrixY, height);

    if (!is_pgm) {
        int cbWidth = h2 * 8, cbHeight = v2 * 8;
        int **matrixCb = creer_matrice(cbWidth, cbHeight);
        fill_subsampled_matrix(matrixCb, input->pixels, width, height, cbWidth, cbHeight, 'C');
        result->Cb_blocks = split_blocks(matrixCb, cbWidth, cbHeight, &result->num_Cb_blocks);
        free_matrix(matrixCb, cbHeight);
        int crWidth = h3 * 8, crHeight = v3 * 8;
        int **matrixCr = creer_matrice(crWidth, crHeight);
        fill_subsampled_matrix(matrixCr, input->pixels, width, height, crWidth, crHeight, 'R');
        result->Cr_blocks = split_blocks(matrixCr, crWidth, crHeight, &result->num_Cr_blocks);
        free_matrix(matrixCr, crHeight);
    }

    return result;
}





// Fonction pour sous-échantillonner une liste de MCUs
MCU_subsampled *subsample_mcus(YCbCrMCU *mcus, int count, int h2, int v2, int h3, int v3,bool is_pgm) {
    MCU_subsampled *subsampled_mcus = malloc(count * sizeof(MCU_subsampled));
    for (int i = 0; i < count; i++) {
        MCU_subsampled *subsampled = subsample(&mcus[i], h2, v2, h3, v3,is_pgm);
        subsampled_mcus[i] = *subsampled;
        free(subsampled);
    }
    return subsampled_mcus;
}

void calculer_nb_blocks(MCU_subsampled *mcus, int mcu_count, bool is_pgm, int *nb_total_Y, int *nb_total_Cb, int *nb_total_Cr) {
    *nb_total_Y = 0;
    *nb_total_Cb = 0;
    *nb_total_Cr = 0;
    for (int i = 0; i < mcu_count; i++) {
        *nb_total_Y += mcus[i].num_Y_blocks;
        if (!is_pgm) {
            *nb_total_Cb += mcus[i].num_Cb_blocks;
            *nb_total_Cr += mcus[i].num_Cr_blocks;
        }
    }
}

void rassembler_blocks(MCU_subsampled *mcus, int mcu_count, bool is_pgm, YCbCrBlock *destination_blocks, int *total_blocks) {
    int nb_total_Y, nb_total_Cb, nb_total_Cr;
    calculer_nb_blocks(mcus, mcu_count, is_pgm, &nb_total_Y, &nb_total_Cb, &nb_total_Cr);
    *total_blocks = nb_total_Y + nb_total_Cb + nb_total_Cr;

    int current_index = 0;
    for (int mcu_index = 0; mcu_index < mcu_count; mcu_index++) {
        for (int y_index = 0; y_index < mcus[mcu_index].num_Y_blocks; y_index++) {
            destination_blocks[current_index].block = mcus[mcu_index].Y_blocks[y_index];
            destination_blocks[current_index].type = 0;  // Y type
            current_index++;
        }
        if (!is_pgm) {
            for (int cb_index = 0; cb_index < mcus[mcu_index].num_Cb_blocks; cb_index++) {
                destination_blocks[current_index].block = mcus[mcu_index].Cb_blocks[cb_index];
                destination_blocks[current_index].type = 1;  // Cb type
                current_index++;
            }
            for (int cr_index = 0; cr_index < mcus[mcu_index].num_Cr_blocks; cr_index++) {
                destination_blocks[current_index].block = mcus[mcu_index].Cr_blocks[cr_index];
                destination_blocks[current_index].type = 2;  // Cr type
                current_index++;
            }
        }
    }
}

YCbCrBlock *assembleBlocks_MCUs(MCU_subsampled *mcus, int mcu_count, int *total_blocks, bool is_pgm) {
    int nb_total_Y, nb_total_Cb, nb_total_Cr;
    calculer_nb_blocks(mcus, mcu_count, is_pgm, &nb_total_Y, &nb_total_Cb, &nb_total_Cr);
    *total_blocks = nb_total_Y + nb_total_Cb + nb_total_Cr;

    YCbCrBlock *blocks = malloc(*total_blocks * sizeof(YCbCrBlock));
    if (blocks == NULL) {
        fprintf(stderr, "ECHEC : allocation mémoire\n");
        return NULL;
    }

    rassembler_blocks(mcus, mcu_count, is_pgm, blocks, total_blocks);
    return blocks;
}







