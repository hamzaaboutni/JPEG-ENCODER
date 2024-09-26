typedef struct {
    char* data;
    size_t size;
    size_t capacity;
} DynamicBuffer;

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int y, cb, cr;
} YCbCrPixel;


typedef struct {
    int width, height;
    Pixel **pixels;
} MCU;

typedef struct {
    YCbCrPixel **pixels;
    int width, height;
} YCbCrMCU;

typedef struct {
    int ***Y_blocks;
    int ***Cb_blocks;
    int ***Cr_blocks;
    int num_Y_blocks;
    int num_Cb_blocks;
    int num_Cr_blocks;
} MCU_subsampled;

typedef struct {
    int **block;
    int type;
} YCbCrBlock;

