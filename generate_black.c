#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Utilisation: %s <largeur> <hauteur> <fichier de sortie>\n", argv[0]);
        return 1;
    }
    int width = atoi(argv[1]), height = atoi(argv[2]);
    FILE *fichier_sortie = fopen(argv[3], "wb");
    fprintf(fichier_sortie, "P5\n%d %d\n255\n", width, height);
    for (int k = 0; k < width *height; k++) {
        if ((k +1)% width == 0) {
            fputc(255,fichier_sortie);
        } else {
            fputc(0, fichier_sortie);
        }
    }
    fclose(fichier_sortie);
    return 0;
}
