#ifndef SOUS_ECHANTILLONAGE_H
#define SOUS_ECHANTILLONAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// fonctions auxilières a des utilités précises
int **alloueur_block();
void free_block(int **block);
int ***split_blocks(int **matrix, int width, int height, int *num_blocks);
void free_YCbCrMCU(YCbCrMCU *mcu);
void free_MCU_subsampled(MCU_subsampled *subsampled);

/* fonction qui prend comme argument un pointeur vers UNE SEULE mcu et renvoie sa version
sous echantillonée.
OUTPUT : Un pointeur vers la mcu sous_echantillonée*/
MCU_subsampled *subsample(YCbCrMCU *input,int h2, int v2, int h3, int v3,bool is_pgm);

/* fonction qui prend comme argument un pointeur vers une LISTE de mcus et renvoie la liste
de mcus sous_échantillonée
OUTPUT : un pointeur vers la liste de mcus sous échantiollonée*/
MCU_subsampled *subsample_mcus(YCbCrMCU *mcus, int count, int h2, int v2, int h3, int v3,bool is_pgm);

/* fonction qui prend comme argument un pointeur vers une liste de mcus sous echantillonée
et renvoie la liste de blocks Y,Cb,Cr ordonnés
OUTPUT : pointeur vers une liste de YCbCrBlocks */
YCbCrBlock *assembleBlocks_MCUs(MCU_subsampled *mcus, int mcu_count, int *total_blocks,bool is_pgm);

#endif // SOUS_ECHANTILLONAGE_H
