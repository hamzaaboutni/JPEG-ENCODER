#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Inclure pour initialiser le générateur de nombres aléatoires
#include "../src/decoupage_mcu.c"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s largeur hauteur largeur_MCU hauteur_MCU\n", argv[0]);
        return EXIT_FAILURE;
    }
    int largeur = atoi(argv[1]);
    int hauteur = atoi(argv[2]);
    int largeur_MCU = atoi(argv[3]);
    int hauteur_MCU = atoi(argv[4]);
    srand(time(NULL));

    // Création de l'image test
    Pixel **image = malloc(hauteur * sizeof(Pixel*));
    for (int i = 0; i < hauteur; i++) {
        image[i] = malloc(largeur * sizeof(Pixel));
        for (int j = 0; j < largeur; j++) {
            //valeurs aléatoires
            image[i][j].r = rand() % 100;
            image[i][j].g = rand() % 100;
            image[i][j].b = rand() % 100;
        }
    }
    printf("=========== Matrice avant découpage =========== :\n\n");
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            printf("(%d,%d,%d) ", image[i][j].r, image[i][j].g, image[i][j].b);
        }
        printf("\n");
    }
    printf("\n");

    MCU **mcus = decouperEnMCU(image, largeur, hauteur, largeur_MCU, hauteur_MCU);
    int nb_mcu_horizontal = (largeur + largeur_MCU - 1) / largeur_MCU;
    int nb_mcu_tical = (hauteur + hauteur_MCU - 1) / hauteur_MCU;

    printf("=========== Matrice après découpage en MCU =========== :\n");
    for (int i = 0; i < nb_mcu_tical; i++) {
        for (int j = 0; j < nb_mcu_horizontal; j++) {
            printf("MCU [%d][%d]:\n", i, j);
            for (int k = 0; k < hauteur_MCU; k++) {
                for (int l = 0; l < largeur_MCU; l++) {
                    printf("(%d,%d,%d) ",
                           mcus[i][j].pixels[k][l].r,
                           mcus[i][j].pixels[k][l].g,
                           mcus[i][j].pixels[k][l].b);
                }
                printf("\n");
            }
            printf("\n");
        }
    }

    for (int i = 0; i < hauteur; i++) {
        free(image[i]);
    }
    free(image);

    for (int i = 0; i < nb_mcu_tical; i++) {
        for (int j = 0; j < nb_mcu_horizontal; j++) {
            for (int k = 0; k < hauteur_MCU; k++) {
                free(mcus[i][j].pixels[k]);
            }
            free(mcus[i][j].pixels);
        }
        free(mcus[i]);
    }
    free(mcus);

    return EXIT_SUCCESS;
}
