#include "../src/lire_fichier_ppm.c"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void afficherUtilisation(const char *prog) {
    printf("Usage : %s fichier_image.ppm\n", prog);
}

void libererMemoireImage(Pixel **image, int hauteur) {
    for (int i = 0; i < hauteur; i++) {
        free(image[i]);
    }
    free(image);
}
// Fonction principale pour le test
int main(int argc, char **argv) {
    if (argc != 2) {
        afficherUtilisation(argv[0]);
        return EXIT_FAILURE;
    }
    int largeur, hauteur;
    bool estPGM;
    Pixel **matriceImage = lire_fichier_ppm(argv[1], &largeur, &hauteur, &estPGM);

    printf("Le fichier %s est lu avec succès.\n", argv[1]);
    printf("Largeur x hauteur = %d x %d\n", largeur, hauteur);

    for (int ligne = 0; ligne < hauteur; ligne++) {
        for (int colonne = 0; colonne < largeur; colonne++) {
            printf("(%3d, %3d, %3d) ", matriceImage[ligne][colonne].r, matriceImage[ligne][colonne].g, matriceImage[ligne][colonne].b);
        }
        printf("\n");
    }
    libererMemoireImage(matriceImage, hauteur);
    return EXIT_SUCCESS;
}
