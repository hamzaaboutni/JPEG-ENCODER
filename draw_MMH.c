#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour dessiner un rectangle dans l'image
void dessiner_rectangle(unsigned char *image, int img_width, int img_height, int x_start, int y_start, int rect_width, int rect_height, unsigned char rouge, unsigned char vert, unsigned char bleu) {
    int y_current= y_start;
    while (y_current< y_start + rect_height) {
        int x_current= x_start;
        while (x_current < x_start + rect_width) {
            if (y_current>= 0 && y_current < img_height && x_current >= 0 && x_current < img_width) {
                size_t index = (y_current * img_width + x_current) * 3;
                image[index] = rouge;
                image[index + 1] = vert;
                image[index + 2] = bleu;
            }
            x_current++;
        }
        y_current++;
    }
}


// Fonction pour dessiner la lettre M
void draw_M(unsigned char *image, int img_width, int img_height, int x, int y, int rect_width, int rect_height, unsigned char r, unsigned char g, unsigned char b) {
    dessiner_rectangle(image,img_width, img_height, x, y, rect_width,rect_height, r, g, b);
    dessiner_rectangle(image,img_width, img_height, x + 2 * rect_width,y, rect_width, rect_height, r, g, b);
    dessiner_rectangle(image,img_width, img_height, x + rect_width,y, rect_width, rect_height / 3, r, g, b);
}

// Fonction pour dessiner la lettre H
void draw_H(unsigned char *image, int img_width, int img_height, int x, int y, int rect_width, int rect_height, unsigned char r, unsigned char g, unsigned char b) {
    dessiner_rectangle(image, img_width,img_height,x, y, rect_width, rect_height, r, g, b);
    dessiner_rectangle(image, img_width, img_height, x + 2 * rect_width, y, rect_width, rect_height, r, g, b);
    dessiner_rectangle(image, img_width, img_height, x + rect_width, y+ rect_height /2 -5, rect_width, rect_width, r, g, b);
}

// Fonction pour générer une couleur aléatoire
void random_color(unsigned char *r, unsigned char *g, unsigned char *b) {
    *r = rand()% 256;
    *g = rand()% 256;
    *b = rand()% 256;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <width> <height> <output_filename>\n", argv[0]);
        return 1;
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    const char *output_filename = argv[3];
    srand(time(NULL));
    unsigned char *image = (unsigned char *)malloc(width * height * 3);
    if (image == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return 1;
    }
    unsigned char bg_r, bg_g, bg_b;
    random_color(&bg_r, &bg_g, &bg_b);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[(i*width + j) * 3] = bg_r;
            image[(i*width + j) * 3 + 1] = bg_g;
            image[(i*width + j) * 3 + 2] = bg_b;
        }
    }
    int rect_width = width / 15;
    int rect_height = height / 3;
    unsigned char r1, g1, b1, r2, g2, b2, r3, g3, b3;
    random_color(&r1,&g1,&b1);
    random_color(&r2,&g2,&b2);
    random_color(&r3,&g3,&b3);
    int x_M1 = rand() % (width -3 * rect_width);
    int y_M1 = rand() % (height - rect_height);
    draw_M(image, width, height, x_M1, y_M1, rect_width, rect_height, r1, g1, b1);
    int x_M2 = rand() % (width -3 * rect_width);
    int y_M2 = rand() % (height - rect_height);
    draw_M(image, width, height, x_M2, y_M2, rect_width, rect_height, r2, g2, b2);
    int x_H = rand() % (width - 3 * rect_width);
    int y_H = rand() % (height - rect_height);
    draw_H(image, width, height, x_H, y_H, rect_width, rect_height, r3, g3, b3);
    FILE *fp = fopen(output_filename, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    fwrite(image, 1, width * height * 3, fp);
    fclose(fp);
    free(image);

    return 0;
}
