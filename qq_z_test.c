#include <stdio.h>
#include "../include/zz_q.h"

#ifndef TAILLE_BASIQUE
#define TAILLE_BASIQUE 8
#endif

void print_block_2d(int block[TAILLE_BASIQUE][TAILLE_BASIQUE]);
void print_block_1d(int *block);

int main() {
    int block[TAILLE_BASIQUE][TAILLE_BASIQUE] = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
    };
    print_block_2d(block);

    int *block_ptrs[TAILLE_BASIQUE];
    for (int i = 0; i < TAILLE_BASIQUE; i++) {
        block_ptrs[i] = block[i];
    }

    int output[64]; 
    convertir_en_zigzag(block_ptrs, output);
    print_block_1d(output);

    return 0;
}
void print_block_2d(int block[TAILLE_BASIQUE][TAILLE_BASIQUE]) {
    printf("Original Block:\n");
    for (int i = 0; i < TAILLE_BASIQUE; i++) {
        for (int j = 0; j < TAILLE_BASIQUE; j++) {
            printf("%3d ", block[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_block_1d(int *block) {
    printf("Zigzag Ordered Block:\n");
    for (int i = 0; i < 64; i++) {
        if (i % TAILLE_BASIQUE == 0 && i != 0)
            printf("\n");
        printf("%3d ", block[i]);
    }
    printf("\n");
}
