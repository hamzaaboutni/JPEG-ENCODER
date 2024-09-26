#ifndef DECOUPAGE_MCU_H
#define DECOUPAGE_MCU_H

#include <stdlib.h>

/* fonction qui prend une image et duplique ses colonnes/lignes pour que ses dimensions soient convenables à la taille de la mcu souhaitée.
OUTPUT : image */
Pixel** dupliquerLignesColonnes(Pixel **image, int largeur, int hauteur, int new_largeur, int new_hauteur);

/* fonction qui prend une image et renvoie une MATRICE de mcus
OUTPUT : mcus */
MCU** decouperEnMCU(Pixel **image, int width, int height, int largeur_MCU, int hauteur_MCU);

/* fonction qui prend comme argument une matrice de MCUs et la renvoie sous forme d'une liste. Par exemple :
un matrice composée de 4x4 mcus appliquée à cette fonction va retourner une liste de 16 mcus ordonnées
de gauche a droite de haut en bas
OUTPUT: MCU* ordonnée a partir d'une matrice d'mcus */
MCU *matriceVersListeMCU(MCU **mcus, int nb_MCU_largeur, int nb_MCU_hauteur);


#endif /* DECOUPAGE_MCU_H */

