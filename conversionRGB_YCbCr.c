#include <stdlib.h>
#include <stdio.h>
#include "../include/SDD.h"
#include <stdbool.h>
#include <math.h>

// Fonction de conversion de RGB à YCbCr
YCbCrPixel rgbToYCbCr(Pixel pixel,bool is_pgm) {
    YCbCrPixel ycbcrPixel;
    if(is_pgm == true){
        ycbcrPixel.y = pixel.r;
        ycbcrPixel.cb = 128;
        ycbcrPixel.cr = 128;
    }
    else{
        ycbcrPixel.y = round((0.299* pixel.r) + (0.587 * pixel.g) + (0.114 * pixel.b));
        ycbcrPixel.cb = round(128 - (0.1687 * pixel.r) - (0.3313 * pixel.g) + (0.5 * pixel.b));
        ycbcrPixel.cr = round(128 + (0.5 * pixel.r) - (0.4187 * pixel.g) - (0.0813 * pixel.b));
    }
    return ycbcrPixel;
}

// Fonction pour convertir une liste de MCU RGB en une liste de YCbCrMCU
YCbCrMCU* convertirMCUs(MCU* mcus, int nombreDeMCUs,bool is_pgm) {
    YCbCrMCU *ycbcrMCUs = malloc(nombreDeMCUs * sizeof(YCbCrMCU));
    for (int i = 0; i < nombreDeMCUs; i++) {
        ycbcrMCUs[i].width = mcus[i].width;
        ycbcrMCUs[i].height = mcus[i].height;
        ycbcrMCUs[i].pixels = malloc(mcus[i].height * sizeof(YCbCrPixel*));
        for (int j = 0; j < mcus[i].height; j++) {
            ycbcrMCUs[i].pixels[j] = malloc(mcus[i].width * sizeof(YCbCrPixel));
            for (int k = 0; k < mcus[i].width; k++) {
                ycbcrMCUs[i].pixels[j][k] = rgbToYCbCr(mcus[i].pixels[j][k],is_pgm);
            }
        }
    }
    return ycbcrMCUs;
}

