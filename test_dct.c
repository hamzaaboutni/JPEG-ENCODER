#include "../include/dct.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int **block;
    int **dctBlock;
    int initial_block[N][N] = {
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128},
        {128, 128, 128, 128, 128, 128, 128, 128}
    };

    block = malloc(N * sizeof(int *));
    dctBlock = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        block[i] = malloc(N * sizeof(int));
        dctBlock[i] = malloc(N * sizeof(int));
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            block[i][j] = initial_block[i][j];
        }
    }
    prepcalcul_cste();
    // Apply DCT
    dct(block, dctBlock);
    printf("Avant DCT:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", block[i][j]);
        }
        printf("\n");
    }
    printf("======================================\n");
    // Print the DCT output
    printf("DCT Output:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", dctBlock[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++) {
        free(block[i]);
        free(dctBlock[i]);
    }
    free(block);
    free(dctBlock);

    return 0;
}