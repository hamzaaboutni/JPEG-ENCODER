#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/SDD.h"

Pixel** lire_fichier_ppm(const char* nom_fichier, int* largeur, int* hauteur, bool *is_pgm) {
    char type[3];
    int i, j;
    int valeur_max;
    Pixel **pixels = NULL;
    FILE* fichier = fopen(nom_fichier, "rb");

    if (!fichier) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    if (fscanf(fichier, "%2s", type) != 1) {
        fprintf(stderr, "Erreur de lecture du type de fichier dans %s.\n", nom_fichier);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    *is_pgm = (type[1] == '5');

    if (type[0] != 'P') {
        fprintf(stderr, "Type de fichier incorrect\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    if (type[1] != '5' && type[1] != '6') {
        fprintf(stderr, "Veuillez donner un fichier de format P5 ou P6.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    int resultat = fscanf(fichier, "%d %d %d%*c", largeur, hauteur, &valeur_max);
    if (resultat == EOF || resultat < 3) {
        fprintf(stderr, "Erreur de format dans les en-têtes de %s. Vérifiez les dimensions et la valeur maximale.\n", nom_fichier);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    if (*hauteur <= 0) {
        fprintf(stderr, "Erreur : La hauteur est négative. %s.\n", nom_fichier);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }
    if (*largeur <= 0) {
        fprintf(stderr, "Erreur : La largeur est négative. %s.\n", nom_fichier);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }
    if (valeur_max > 255) {
        fprintf(stderr, "Valeur maximale invalide dans %s. Attendu 255.\n", nom_fichier);
        fclose(fichier);
        exit(EXIT_FAILURE);
    }
    pixels = (Pixel**)malloc(*hauteur * sizeof(Pixel*));
    for (i = 0; i < *hauteur; i++) {
        pixels[i] = (Pixel*)malloc(*largeur * sizeof(Pixel));
    }

    for (i = 0; i < *hauteur; i++) {
        for (j = 0; j < *largeur; j++) {
            if (type[1] == '6') {
                fread(&pixels[i][j], sizeof(Pixel), 1, fichier);
            } else {
                fread(&pixels[i][j].r, 1, 1, fichier);
                pixels[i][j].g = pixels[i][j].b = pixels[i][j].r;
            }
        }
    }

    fclose(fichier);
    return pixels;
}
