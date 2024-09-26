#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/SDD.h"
#include "../include/htables.h"


char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}


// Initialise le tampon dynamique
void initBuffer(DynamicBuffer* buffer, size_t initialCapacity) {
    buffer->data = malloc(initialCapacity);
    if (buffer->data == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }
    buffer->size = 0;
    buffer->capacity = initialCapacity;
}

// Libère la mémoire du tampon dynamique
void freeBuffer(DynamicBuffer* buffer) {
    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

// Ajoute une chaîne de caractères au tampon dynamique
void appendToBuffer(DynamicBuffer* buffer, const char* str) {
    size_t strLen = strlen(str);
    if (buffer->size + strLen + 1 > buffer->capacity) {
        buffer->capacity *= 2;
        buffer->data = realloc(buffer->data, buffer->capacity);
        if (buffer->data == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            exit(1);
        }
    }
    strcpy(buffer->data + buffer->size, str);
    buffer->size += strLen;
}

// Calcule la classe de magnitude pour une valeur donnée
int calculate_magnitude_class(int value) {
    if (value == 0) return 0;
    return (int)log2(abs(value)) + 1;
}

// Fonction pour convertir un entier en chaîne binaire avec un nombre fixe de bits
void decimal_to_binary(int decimal, int bits, char *binary) {
    binary[bits] = '\0';  // Termine la chaîne
    for (int i = bits - 1; i >= 0; i--) {
        binary[i] = (decimal % 2) + '0';
        decimal /= 2;
    }
}

// Calcule l'indice en binaire pour une valeur donnée et une classe de magnitude
void find_magnitude_index(int value, int magnitude, char *binary_index) {
    if (magnitude == 0) {
        strcpy(binary_index, "0");
        return;
    }

    int min_neg = -(1 << magnitude);
    int max_pos = (1 << magnitude) - 1;
    int min_pos = 1 << (magnitude - 1);

    int decimal_index;
    if (value < 0) {
        decimal_index = (value - min_neg) - 1;
    } else {
        decimal_index = (value - min_pos) + (max_pos - min_pos + 1);
    }

    decimal_to_binary(decimal_index, magnitude, binary_index);
}

// Génère le code Huffman pour un symbole cible
void generateHuffmanCode(uint8_t symbol_lengths[16], uint8_t symbols[], int numSymbols, uint8_t target, char *codeOut) {
    uint32_t code = 0;
    int index = 0;  // Déclaration et initialisation de la variable index
    int bitPos = 0;  // Position du bit dans le code de sortie

    for (int len = 1; len <= MAX_LENGTH; len++) {
        for (int i = 0; i < symbol_lengths[len - 1]; i++) {
            if (index >= numSymbols) return;  // Plus de symboles que prévu
            if (symbols[index] == target) {
                // Ajouter les bits du code Huffman à partir du plus significatif
                for (int j = len - 1; j >= 0; j--) {
                    // Utilisation d'opérations bit à bit pour extraire les bits du code
                    codeOut[bitPos++] = ((code >> j) & 1) + '0';
                }
                codeOut[bitPos] = '\0';  // Terminer la chaîne de caractères
                return;
            }
            index++;
            code++;
        }
        code <<= 1;  // Décalage vers la gauche pour ajouter un nouveau bit
    }
    codeOut[0] = '\0';  // Aucun code trouvé, la chaîne de sortie reste vide
}

// Fonction pour l'encodage DC des composantes Y
char* encodage_DC_Y(int value) {
    // Calcul de la magnitude
    int magnitude = calculate_magnitude_class(value);

    // Obtenir l'indice binaire
    char binary_index[33];
    find_magnitude_index(value, magnitude, binary_index);

    // Générer le code Huffman pour la classe de magnitude
    char huffmanCode[32];
    generateHuffmanCode(htables_nb_symb_per_lengths[0][0], htables_symbols[0][0], htables_nb_symbols[0][0], magnitude, huffmanCode);

    // Utiliser DynamicBuffer pour créer le résultat
    DynamicBuffer buffer;
    initBuffer(&buffer, 64);  // Capacité initiale arbitraire suffisante

    appendToBuffer(&buffer, huffmanCode);
    if (value != 0) {
        appendToBuffer(&buffer, binary_index);
    }

    char* fullDCCode = strdup(buffer.data);
    freeBuffer(&buffer);

    return fullDCCode;  // Retourne le codage complet
}

// Fonction pour l'encodage DC des composantes Cb ou Cr
char* encodage_DC_CbCr(int value) {
    // Calcul de la magnitude
    int magnitude = calculate_magnitude_class(value);

    // Obtenir l'indice binaire
    char binary_index[33];
    find_magnitude_index(value, magnitude, binary_index);

    // Générer le code Huffman pour la classe de magnitude
    char huffmanCode[32];
    generateHuffmanCode(htables_nb_symb_per_lengths[0][1], htables_symbols[0][1], htables_nb_symbols[0][1], magnitude, huffmanCode);

    // Utiliser DynamicBuffer pour créer le résultat
    DynamicBuffer buffer;
    initBuffer(&buffer, 64);  // Capacité initiale arbitraire suffisante

    appendToBuffer(&buffer, huffmanCode);
    if (value != 0) {
        appendToBuffer(&buffer, binary_index);
    }

    char* fullDCCode = strdup(buffer.data);
    freeBuffer(&buffer);

    return fullDCCode;  // Retourne le codage complet
}


char* encodage_AC_Y(int ac_values[63], int *count) {
    DynamicBuffer buffer;
    initBuffer(&buffer, 1024);

    int zero_run = 0;
    int index = 0;

    for (int i = 0; i < 63; i++) {
        if (ac_values[i] == 0) {
            zero_run++;
            if (zero_run == 16) {
                int has_non_zero = 0;
                for (int j = i; j < 63; j++) {
                    if (ac_values[j] != 0) {
                        has_non_zero = 1;
                        char codeOut[32];
                        generateHuffmanCode(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0], 0xF0, codeOut);
                        appendToBuffer(&buffer, codeOut);
                        appendToBuffer(&buffer, " ");
                        zero_run = 0;
                        break;
                    }
                }
                if (!has_non_zero) {
                    char codeOut[32];
                    generateHuffmanCode(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0], 0x00, codeOut);
                    appendToBuffer(&buffer, codeOut);
                    zero_run = 0;
                    break;
                }
            }
        } else {
            int magnitude = calculate_magnitude_class(ac_values[i]);
            uint8_t symbol = (zero_run << 4) | magnitude;
            char codeOut[32];
            generateHuffmanCode(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0], symbol, codeOut);

            appendToBuffer(&buffer, codeOut);
            appendToBuffer(&buffer, " ");

            char binary_index[32];
            find_magnitude_index(ac_values[i], magnitude, binary_index);
            appendToBuffer(&buffer, binary_index);
            appendToBuffer(&buffer, " ");

            zero_run = 0;
            index++;
        }
    }

    if (zero_run > 0) {
        char codeOut[32];
        generateHuffmanCode(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0], 0x00, codeOut);
        appendToBuffer(&buffer, codeOut);
    }

    *count = index;

    // Transférer les données à une nouvelle chaîne et nettoyer
    char* final_stream = strdup(buffer.data);
    freeBuffer(&buffer);

    return final_stream;
}

char* encodage_AC_CbCr(int ac_values[63], int *count) {
    DynamicBuffer buffer;
    initBuffer(&buffer, 1024);

    int zero_run = 0;
    int index = 0;

    for (int i = 0; i < 63; i++) {
        if (ac_values[i] == 0) {
            zero_run++;
            if (zero_run == 16) {
                int has_non_zero = 0;
                for (int j = i; j < 63; j++) {
                    if (ac_values[j] != 0) {
                        has_non_zero = 1;
                        char codeOut[32];
                        generateHuffmanCode(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1], 0xF0, codeOut);
                        appendToBuffer(&buffer, codeOut);
                        appendToBuffer(&buffer, " ");
                        zero_run = 0;
                        break;
                    }
                }
                if (!has_non_zero) {
                    char codeOut[32];
                    generateHuffmanCode(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1], 0x00, codeOut);
                    appendToBuffer(&buffer, codeOut);
                    zero_run = 0;
                    break;
                }
            }
        } else {
            int magnitude = calculate_magnitude_class(ac_values[i]);
            uint8_t symbol = (zero_run << 4) | magnitude;
            char codeOut[32];
            generateHuffmanCode(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1], symbol, codeOut);

            appendToBuffer(&buffer, codeOut);
            appendToBuffer(&buffer, " ");

            char binary_index[32];
            find_magnitude_index(ac_values[i], magnitude, binary_index);
            appendToBuffer(&buffer, binary_index);
            appendToBuffer(&buffer, " ");

            zero_run = 0;
            index++;
        }
    }

    if (zero_run > 0) {
        char codeOut[32];
        generateHuffmanCode(htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1], 0x00, codeOut);
        appendToBuffer(&buffer, codeOut);
    }

    *count = index;

    // Transférer les données à une nouvelle chaîne et nettoyer
    char* final_stream = strdup(buffer.data);
    freeBuffer(&buffer);

    return final_stream;
}

// Cette fonction convertit la matrice 8x8 en un flux de bytes (chaîne de caractères).
char* encoder_matrice_Y(int** matrice) {
    char* dc_encoding = encodage_DC_Y(matrice[0][0]);
    if (dc_encoding == NULL) return NULL;

    DynamicBuffer buffer;
    initBuffer(&buffer, 1024);

    int ac_values[63];
    int index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!(i == 0 && j == 0)) {
                ac_values[index++] = matrice[i][j];
                if (index == 63) break;
            }
        }
    }

    int count_ac;
    char* ac_encoding = encodage_AC_Y(ac_values, &count_ac);
    if (ac_encoding == NULL) {
        free(dc_encoding);
        return NULL;
    }

    appendToBuffer(&buffer, dc_encoding);
    appendToBuffer(&buffer, " ");
    appendToBuffer(&buffer, ac_encoding);

    free(dc_encoding);
    free(ac_encoding);

    // Transférer les données à une nouvelle chaîne et nettoyer
    char* final_stream = strdup(buffer.data);
    freeBuffer(&buffer);

    return final_stream;
}

// Cette fonction convertit la matrice 8x8 pour les composantes Cb ou Cr en un flux de bytes.
char* encoder_matrice_CbCr(int** matrice) {
    char* dc_encoding = encodage_DC_CbCr(matrice[0][0]);
    if (dc_encoding == NULL) return NULL;

    DynamicBuffer buffer;
    initBuffer(&buffer, 1024);

    int ac_values[63];
    int index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!(i == 0 && j == 0)) {
                ac_values[index++] = matrice[i][j];
                if (index == 63) break;
            }
        }
    }

    int count_ac;
    char* ac_encoding = encodage_AC_CbCr(ac_values, &count_ac);
    if (ac_encoding == NULL) {
        free(dc_encoding);
        return NULL;
    }

    appendToBuffer(&buffer, dc_encoding);
    appendToBuffer(&buffer, " ");
    appendToBuffer(&buffer, ac_encoding);

    free(dc_encoding);
    free(ac_encoding);

    // Transférer les données à une nouvelle chaîne et nettoyer
    char* final_stream = strdup(buffer.data);
    freeBuffer(&buffer);

    return final_stream;
}

// Fonction principale pour appliquer Huffman à chaque bloc et concaténer les flux de bits
char* Huffman(YCbCrBlock *blocks, int numBlocks) {
    // Initialisation des valeurs des DC précédents
    int prec0 = 0, prec1 = 0, prec2 = 0;

    DynamicBuffer buffer;
    initBuffer(&buffer, 1024 * numBlocks);

    for (int i = 0; i < numBlocks; i++) {
        int **block = blocks[i].block;
        int type = blocks[i].type;

        // Soustraction du DC précédent en fonction du type de bloc
        int prevDC;
        switch (type) {
            case 0:  // Y block
                prevDC = prec0;
                prec0 = block[0][0];  // Mise à jour du DC précédent pour les blocs Y
                break;
            case 1:  // Cb block
                prevDC = prec1;
                prec1 = block[0][0];  // Mise à jour du DC précédent pour les blocs Cb
                break;
            case 2:  // Cr block
                prevDC = prec2;
                prec2 = block[0][0];  // Mise à jour du DC précédent pour les blocs Cr
                break;
            default:
                printf("Type de bloc non valide\n");
                freeBuffer(&buffer);
                return NULL;
        }

        // Soustraction du DC précédent du DC actuel
        block[0][0] -= prevDC;

        // Encodage du bloc en fonction du type
        char *block_stream;
        if (type == 0) {
            block_stream = encoder_matrice_Y(block);
        } else {
            block_stream = encoder_matrice_CbCr(block);
        }

        if (block_stream != NULL) {
            appendToBuffer(&buffer, block_stream);
            appendToBuffer(&buffer, " ");
            free(block_stream);
        } else {
            printf("Encodage du bloc de type %d a échoué.\n", type);
        }
    }

    // Transférer les données à une nouvelle chaîne et nettoyer
    char* final_stream = strdup(buffer.data);
    freeBuffer(&buffer);

    return final_stream;
}

// Fonction pour afficher un bloc YCbCr
void print_block(YCbCrBlock block) {
    printf("Type : %d\n", block.type);
    printf("Block :\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%4d", block.block[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
