#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdint.h>
int calculate_magnitude_class(int value);
void decimal_to_binary(int decimal, int bits, char *binary);
void find_magnitude_index(int value, int magnitude, char *binary_index);
void generateHuffmanCode(uint8_t symbol_lengths[16], uint8_t symbols[], int numSymbols, uint8_t target, char *codeOut);
char* encodage_DC_Y(int value);
char* encodage_DC_CbCr(int value);
char* encodage_AC_Y(int ac_values[63], int *count);
char* encodage_AC_CbCr(int ac_values[63], int *count);
char* encoder_matrice_Y(int** matrice);
char* encoder_matrice_CbCr(int** matrice);
char* Huffman(YCbCrBlock *blocks, int numBlocks);
void print_block(YCbCrBlock block);
#endif // HUFFMAN_H
