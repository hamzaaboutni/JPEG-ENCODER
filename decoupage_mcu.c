#include <stdio.h>
#include <stdlib.h>
#include "../include/SDD.h"
#include "../include/decoupage_mcu.h"

void dupliquerImage(Pixel **original, Pixel **dupliquee, int largeur_orig, int hauteur_orig, int largeur_nouvelle, int hauteur_nouvelle) {
    for (int ligne = 0; ligne < hauteur_nouvelle; ligne++) {
        for (int colonne = 0; colonne < largeur_nouvelle; colonne++) {
            int ligne_source, colonne_source;
            if (ligne < hauteur_orig) {
                ligne_source =ligne;
            } else {
                ligne_source = hauteur_orig- 1;
            }
            if (colonne < largeur_orig) {
                colonne_source = colonne;
            } else {
                colonne_source =largeur_orig - 1;
            }

            dupliquee[ligne][colonne] =original[ligne_source][colonne_source];
        }
    }
}

Pixel** creerImage(int largeur, int hauteur) {
    Pixel **image = (Pixel **)malloc(hauteur * sizeof(Pixel *));
    for (int i = 0; i < hauteur; i++) {
        image[i] = (Pixel *)malloc(largeur * sizeof(Pixel));
    }
    return image;
}

Pixel** redimensionnerImage(Pixel **image,int largeur, int hauteur, int largeur_nouvelle, int hauteur_nouvelle) {
    Pixel **image_dupliquee = creerImage(largeur_nouvelle, hauteur_nouvelle);
    dupliquerImage(image, image_dupliquee,largeur, hauteur, largeur_nouvelle, hauteur_nouvelle);
    return image_dupliquee;
}

void ajuster_les_dimensions(int width, int height, int blockWidth, int blockHeight, int *newWidth, int *newHeight) {
    if (width % blockWidth== 0){
        *newWidth = width;
    } else{
        *newWidth = ((width / blockWidth) + 1) * blockWidth;
    }
    if (height % blockHeight == 0) {
        *newHeight = height;
    } else{
        *newHeight =((height / blockHeight) + 1) * blockHeight;
    }
}

MCU allouer_memoire_mcus(int width, int height) {
    MCU mcu;
    mcu.width = width;
    mcu.height = height;
    mcu.pixels = (Pixel **)malloc(height * sizeof(Pixel *));
    for (int i = 0; i < height; i++) {
        mcu.pixels[i]= (Pixel *)malloc(width* sizeof(Pixel));
    }
    return mcu;
}

void extraction_de_mcus(Pixel **image, MCU *mcu,int startX, int startY) {
    for (int y = 0; y < mcu->height; y++) {
        for (int x = 0; x < mcu->width; x++) {
            mcu->pixels[y][x] = image[startY + y][startX + x];
        }
    }
}

MCU** decouperEnMCU(Pixel **image, int largeur_de_limage,int hauteur_de_limage, int largeur_MCU, int hauteur_MCU) {
    int largeur_dupliquee, hauteur_dupliquee;
    ajuster_les_dimensions(largeur_de_limage, hauteur_de_limage, largeur_MCU, hauteur_MCU, &largeur_dupliquee, &hauteur_dupliquee);
    if (largeur_dupliquee != largeur_de_limage || hauteur_dupliquee != hauteur_de_limage) {
        image = redimensionnerImage(image, largeur_de_limage,hauteur_de_limage, largeur_dupliquee, hauteur_dupliquee);
        largeur_de_limage = largeur_dupliquee;
        hauteur_de_limage = hauteur_dupliquee;
    }
    int nb_mcu_horizontal =largeur_de_limage /largeur_MCU;
    int nb_mcu_vertical = hauteur_de_limage /hauteur_MCU;

    MCU **output = (MCU **)malloc(nb_mcu_vertical * sizeof(MCU *));
    for (int row = 0; row < nb_mcu_vertical; row++) {
        output[row] = (MCU *)malloc(nb_mcu_horizontal * sizeof(MCU));
        for (int col = 0; col < nb_mcu_horizontal; col++) {
            output[row][col] = allouer_memoire_mcus(largeur_MCU,hauteur_MCU);
            extraction_de_mcus(image, &output[row][col], col * largeur_MCU, row * hauteur_MCU);
        }
    }
    return output;
}

MCU *matriceVersListeMCU(MCU **output, int nb_mcu_horizontal, int nb_mcu_vertical) {
    int totaloutput = nb_mcu_horizontal *nb_mcu_vertical;
    MCU *listeoutput = malloc(totaloutput* sizeof(MCU));
    for (int i = 0, count = 0; i < nb_mcu_vertical; i++) {
        for (int j = 0; j < nb_mcu_horizontal; j++) {
            listeoutput[count++] = output[i][j];
        }
    }
    return listeoutput;
}
