#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../src/sous_echantillonage.c"

void libererMemoireYCbCr(YCbCrMCU *image) {
    for (int i = 0; i < image->height;i++) {
        free(image->pixels[i]);
    }
    free(image->pixels);
}
void libererMemoireSubsamples(MCU_subsampled *subsampled) {
    free(subsampled);
}

int main() {
    bool estPGM = false;
    int y_horiz, y_vert, cb_horiz, cb_vert, cr_horiz, cr_vert;

    printf("Entrez les paramètres h1, v1 pour Y: ");
    scanf("%d %d", &y_horiz, &y_vert);
    printf("Entrez les paramètres h2, v2 pour Cb: ");
    scanf("%d %d", &cb_horiz, &cb_vert);
    printf("Entrez les paramètres h3, v3 pour Cr: ");
    scanf("%d %d", &cr_horiz, &cr_vert);
    srand(time(NULL));
    int largeur = y_horiz * 8;
    int hauteur = y_vert * 8;
    YCbCrMCU imageOriginale;
    imageOriginale.width = largeur;
    imageOriginale.height = hauteur;
    imageOriginale.pixels = malloc(hauteur * sizeof(YCbCrPixel *));
    for (int i = 0; i < hauteur; i++) {
        imageOriginale.pixels[i] = malloc(largeur * sizeof(YCbCrPixel));
        for (int j = 0; j < largeur; j++) {
            imageOriginale.pixels[i][j].y =rand() % 10;
            imageOriginale.pixels[i][j].cb =rand() % 10;
            imageOriginale.pixels[i][j].cr =rand() % 10;
        }
    }
    printf("Image originale:\n");
    for (int i = 0; i < hauteur;i++) {
        for (int j = 0; j < largeur;j++) {
            printf("(%d,%d,%d) ", imageOriginale.pixels[i][j].y, imageOriginale.pixels[i][j].cb, imageOriginale.pixels[i][j].cr);
        }
        printf("\n");
    }
    printf("\n");

    MCU_subsampled *imageSousEchantillonnee = subsample(&imageOriginale,cb_horiz, cb_vert, cr_horiz, cr_vert, estPGM);

    printf("Blocs Y:\n");
    for (int i = 0; i < imageSousEchantillonnee->num_Y_blocks; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                printf("%d ",imageSousEchantillonnee->Y_blocks[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("Blocs Cb:\n");
    for (int i = 0; i <imageSousEchantillonnee->num_Cb_blocks; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0;k < 8; k++) {
                printf("%d ", imageSousEchantillonnee->Cb_blocks[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("Blocs Cr:\n");
    for (int i = 0; i < imageSousEchantillonnee->num_Cr_blocks; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                printf("%d ", imageSousEchantillonnee->Cr_blocks[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    libererMemoireYCbCr(&imageOriginale);
    libererMemoireSubsamples(imageSousEchantillonnee);

    return 0;
}
