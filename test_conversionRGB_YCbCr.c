#include "../src/conversionRGB_YCbCr.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

bool isPGM = false;

void releasePixelArray(Pixel **pixelArray, int numRows) {
    for (int i = 0; i < numRows; i++) {
        free(pixelArray[i]);
    }
    free(pixelArray);
}

int main() {
    srand(time(NULL));
    MCU image;
    image.height = 3;
    image.width = 3;
    image.pixels = malloc(image.height * sizeof(Pixel *));

    for (int row = 0; row < image.height; row++) {
        image.pixels[row] = malloc(image.width * sizeof(Pixel));
        if (image.pixels[row] == NULL) {
            fprintf(stderr, "Erreur : allocation mémoire");
            exit(EXIT_FAILURE);
        }

        for (int col = 0; col < image.width; col++) {
            image.pixels[row][col].r = rand() % 256;
            image.pixels[row][col].g = rand() % 256;
            image.pixels[row][col].b = rand() % 256;
        }
    }
    printf("============== Initial RGB Matrix ============\n");
    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            printf("(%d,%d,%d) ", image.pixels[row][col].r, image.pixels[row][col].g, image.pixels[row][col].b);
        }
        printf("\n");
    }
    printf("==================================================\n\n");
    YCbCrMCU *ycbcrImage = convertirMCUs(&image, 1, isPGM);
    printf("============== Converted YCbCr Matrix ============\n");
    for (int row = 0; row < ycbcrImage->height; row++) {
        for (int col = 0; col < ycbcrImage->width; col++) {
            printf("(%d,%d,%d) ", ycbcrImage->pixels[row][col].y, ycbcrImage->pixels[row][col].cb, ycbcrImage->pixels[row][col].cr);
        }
        printf("\n");
    }
    printf("==================================================\n");

    releasePixelArray(image.pixels, image.height);
    releasePixelArray((Pixel **)ycbcrImage->pixels, ycbcrImage->height);
    free(ycbcrImage);

    return 0;
}
