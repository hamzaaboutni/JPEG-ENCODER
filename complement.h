#ifndef COMPLEMENT_H
#define COMPLEMENT_H

#include <stdio.h>
#include <stdlib.h>

int **transformer_1D_2D(int *array);
void zzq_liste_block(YCbCrBlock* liste_blocks, int total_blocks);
YCbCrBlock *convert_blocks_to_zigzag(const YCbCrBlock *liste_blocks, int total_blocks);
void write_to_file(const char* filename, const char* bitstream);

#endif /* COMPLEMENT_H */
