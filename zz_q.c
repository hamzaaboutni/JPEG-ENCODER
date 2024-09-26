#include "../include/zz_q.h" 

const int coordonnees_ZZ[64][2] = {
    {0,0},{0,1},{1,0},{2,0},{1,1},{0,2},{0,3},{1,2},
    {2,1},{3,0},{4,0},{3,1},{2,2},{1,3},{0,4},{0,5},
    {1,4},{2,3},{3,2},{4,1},{5,0},{6,0},{5,1},{4,2},
    {3,3},{2,4},{1,5},{0,6},{0,7},{1,6},{2,5},{3,4},
    {4,3},{5,2},{6,1},{7,0},{7,1},{6,2},{5,3},{4,4},
    {3,5},{2,6},{1,7},{2,7},{3,6},{4,5},{5,4},{6,3},
    {7,2},{7,3},{6,4},{5,5},{4,6},{3,7},{4,7},{5,6},
    {6,5},{7,4},{7,5},{6,6},{5,7},{6,7},{7,6},{7,7}
};
// rearrengement d'un blok en zigzag:

void convertir_en_zigzag(int **input, int *ZZ_output) {
    for (int idx=0;idx< 64;idx++) {
         int ligne = coordonnees_ZZ[idx][0];
        int col =   coordonnees_ZZ[idx][1];
        ZZ_output[idx]=input[ligne][col];
    }
}

void quantification(int *block, const uint8_t *quantification_table) {
    for (int idx= 0;idx<64;idx++) {
        block[idx]=block[idx]/quantification_table[idx];
    }
}

