#ifndef CONVERSION_RGB_YCBCR_H
#define CONVERSION_RGB_YCBCR_H

#include <stdlib.h>
#include <stdio.h>

/* fonction : prend comme argument un seul pixel et le transforme un YCbCrPixel
OUTPUT : YCbCrpixel transformé*/
YCbCrPixel rgbToYCbCr(Pixel pixel,bool is_pgm);

/*fonction : prend comme argument une liste de mcus et transforme chaque mcu en une YCbCrMCU mcu
OUTPUT : liste de YCbCrMCU */
YCbCrMCU* convertirMCUs(MCU* mcus, int nombreDeMCUs, bool is_pgm);

#endif /* CONVERSION_RGB_YCBCR_H */
