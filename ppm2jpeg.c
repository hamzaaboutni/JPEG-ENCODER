#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/SDD.h"
#include "../include/lire_fichier_ppm.h"
#include "../include/decoupage_mcu.h"
#include "../include/conversionRGB_YCbCr.h"
#include "../include/sous_echantillonage.h"
#include "../include/complement.h"
#include "../include/huffman.h"
#include "../include/jpeg_header.h"
#include <time.h>
#include "../include/dct.h"

void showusage() {
    printf("Utilisation du programme de conversion PPM vers JPEG\n");
    printf("\nOptions:\n");
    printf("--help                     : Pour afficher ce message d'aide.\n");
    printf("--outfile=<fichier>        : Pour spécifier le nom du fichier JPEG de sortie.\n");
    printf("              Par défaut   : Le fichier de sortie est 'output.jpg'.\n");
    printf("--sample=h1xv1,h2xv2,h3xv3 : Pour définir les facteurs d'échantillonnage pour les composantes couleur.\n");
    printf("Usage:\n");
    printf("  ppm2jpeg [options] <fichier_entree.ppm>\n");
}

bool compris_entre_4_1(int h, int v) {
    return (h >= 1 && h <= 4 && v >= 1 && v <= 4);
}
bool somme_inf_10(int h1, int v1, int h2, int v2, int h3, int v3) {
    return (h1 *v1 +h2 * v2 + h3*v3 <= 10);
}
bool divisibilite(int h1, int v1, int h2, int v2, int h3, int v3) {
    return (h1%h2 == 0 && h1 % h3 == 0 && v1%v2 == 0 && v1%v3 == 0);
}
int verifierFacteursEchantillonnage(int h1, int v1, int h2, int v2, int h3, int v3) {
    if (!compris_entre_4_1(h1, v1) || !compris_entre_4_1(h2, v2) || !compris_entre_4_1(h3, v3)) {
        printf("Verifiez que les hi vi sont compris entre 1 et 4\n");
        return 0;
    }
    if (!somme_inf_10(h1, v1, h2, v2, h3, v3)) {
        printf("Verifiez que la somme des hi x vi est inférieure ou égale a 10\n");
        return 0;
    }
    if (!divisibilite(h1, v1, h2, v2, h3, v3)) {
        printf("Verifiez que les hi divisent h1 et que les vi divisent v1\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    prepcalcul_cste();
    if (argc < 2) {
        showusage();
        return 1;
    }
    clock_t start_time=clock();
    char *input_filename = NULL;
    char output_filename[255] = "output.jpg";
    int h1 = 1, v1 = 1, h2 = 1, v2 = 1, h3 = 1, v3 = 1;

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "--help") == 0) {
            showusage();
            return 0;
        }

        if (strncmp(arg, "--outfile=", 10) == 0) {
            char *filenameStart = arg + 10;
            if (*filenameStart == '\0') {
                fprintf(stderr, "Erreur: Aucun nom de fichier spécifié pour l'option '--outfile='.\n");
                return 1;
            }
            strncpy(output_filename, filenameStart, sizeof(output_filename) - 1);
            output_filename[sizeof(output_filename) - 1] = '\0';

            continue;
        }
        if (strncmp(arg, "--sample=", 9) == 0) {
            char *samplingParams = arg + 9;

            int parsedItems = sscanf(samplingParams, "%dx%d,%dx%d,%dx%d",
                             &h1, &v1, &h2, &v2, &h3, &v3);

            if (parsedItems != 6) {
                fprintf(stderr, "Erreur: Format des facteurs d'échantillonnage incorrect.\n");
                return 1;
            }

            if (!verifierFacteursEchantillonnage(h1, v1, h2, v2, h3, v3)) {
                fprintf(stderr, "Facteurs d'échantillonnage invalides.\n");
                return 1;
            }
            continue;
        }
        if (!input_filename && arg[0] != '-') {
            input_filename = arg;
        } else {
            fprintf(stderr, "Erreur: Ficher non reconnu ou plusieurs arguments ont été donnés: '%s'.\n", arg);
            return 1;
        }
    }

    if (!input_filename) {
        fprintf(stderr, "Erreur: Aucun fichier n'est spécifié.\n");
        return 1;
    }

    // Lecture du fichier ppm ou pgm
    int largeur, hauteur;
    bool is_pgm;
    Pixel **matrice_pixels = lire_fichier_ppm(input_filename, &largeur, &hauteur,&is_pgm);
    if (!matrice_pixels) {
        fprintf(stderr, "Erreur: Échec de la lecture du fichier PPM '%s'.\n", input_filename);
        return 1;
    }
    if(is_pgm == true){
        h1 = v1 = h2 = v2 = h3 = v3 = 1;
    }
    int mcu_width=8*h1;
    int mcu_height=8*v1;

    MCU** matrice_mcus = decouperEnMCU(matrice_pixels, largeur, hauteur, mcu_width, mcu_height);
    int largeur_2 = (largeur%mcu_width == 0) ? largeur : ((largeur/mcu_width + 1) * mcu_width);
    int hauteur_2 = (hauteur%mcu_height == 0) ? hauteur : ((hauteur/mcu_height + 1) * mcu_height);
    int nb_MCU_hauteur=hauteur_2/mcu_height;
    int nb_MCU_largeur=largeur_2/mcu_width;
    MCU *liste_mcus = matriceVersListeMCU(matrice_mcus, nb_MCU_largeur, nb_MCU_hauteur);

    int nb_mcus=nb_MCU_hauteur*nb_MCU_largeur;
    YCbCrMCU *liste_mcus_YCbCr = convertirMCUs(liste_mcus,nb_mcus,is_pgm);

    MCU_subsampled *liste_mcus_subsampled = subsample_mcus(liste_mcus_YCbCr, nb_mcus,h2,v2,h3,v3,is_pgm);
    int total_blocks;
    YCbCrBlock *liste_blocks=assembleBlocks_MCUs(liste_mcus_subsampled, nb_mcus, &total_blocks,is_pgm);
     /*jusqu'ici, on a une liste de blocks ordonnées sur laquelle on va appliqeur le DCT et le zz+Q*/
    for (int i = 0; i < total_blocks; i++) {
        int *dataBlock = malloc(N * N * sizeof(int));
        int **dctBlock = malloc(N * sizeof(int*));
        for (int j = 0; j < N; j++) {
            dctBlock[j] = dataBlock + j * N;
        }

        // Appel de la fonction DCT
        dct(liste_blocks[i].block, dctBlock);

        free(liste_blocks[i].block[0]);
        free(liste_blocks[i].block);
        liste_blocks[i].block = dctBlock;
    }
    zzq_liste_block(liste_blocks,total_blocks);
    char* bitstream = Huffman(liste_blocks,total_blocks);
    //write_to_file("bitstream.txt",bitstream);
    size_t length;
    write_jpeg_file(output_filename,largeur,hauteur, is_pgm,  h1,  v1,  h2,  v2,  h3,  v3, bitstream, &length);

    for (int i = 0; i < hauteur; i++) {
        free(matrice_pixels[i]);
    }
    free(matrice_pixels);
    clock_t end_time = clock();
    double total_time = (double)(end_time-start_time)/CLOCKS_PER_SEC;

    // Affichage du temps total
    printf("Temps total d'exécution: %f secondes\n", total_time);

    return 0;

}