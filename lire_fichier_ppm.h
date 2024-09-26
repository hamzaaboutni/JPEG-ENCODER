#ifndef LIRE_FICHIER_PPM_H
#define LIRE_FICHIER_PPM_H

#include <stdio.h>

// fonction pour lire un fichier PPM
Pixel** lire_fichier_ppm(const char* nom_fichier, int* largeur, int* hauteur,bool *is_pgm);

#endif /* LIRE_FICHIER_PPM_H */
