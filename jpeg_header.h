#ifndef JPEG_WRITER_H
#define JPEG_WRITER_H

#include "../include/jpeg_header.h"
#include "../include/htables.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//differents marqueurs:
void write_markeur(FILE *file, uint16_t markeur);
void write_eoi(FILE *file);
void write_soi(FILE *file);
void write_app0_header(FILE *file);
void write_DQT_SECTION(FILE *file);
void write_DQT_Y(FILE *file);
void write_DQT_CbCr(FILE *file);
void write_DHT_section(FILE *file, uint8_t type, uint8_t index, uint8_t *nBR_lengths, uint8_t *symbols, uint16_t total_symbols);
void write_dht(FILE *file);
void write_sof0_gris(FILE *file, int width, int height);
void write_sof0_couleur(FILE *file, int width, int height, int h1, int v1, int h2, int v2, int h3, int v3);
void write_sos_couleur(FILE *file);
void write_sos_gris(FILE *file);
//ecrire les donnees encodes avec stuffing
void write_octet_avec_stuffing(FILE *file, unsigned char octet);
unsigned char convertir_bitstring_en_octet(const char *bitstring);
void traiter_et_ecrire_octet(FILE *fichier, const char *bitstream, size_t offset);
void traiter_et_ecrire_bits_restants(FILE *fichier, const char *flux_bits, size_t offset, size_t bits_restants);
void write_data_encode_avec_stuffing(FILE *file, const char *bitstream, size_t length);
//generateur de bitstream
size_t calcul_longueur_bitstream(const char *data);
void remplir_bitstream(const char *data, char *bitstream) ;
char* generate_bitstream(const char *data, size_t *longueur);
//fonction principale
void write_jpeg_file(const char *filename, int width, int height, bool is_pgm, int h1, int v1, int h2, int v2, int h3, int v3, const char *data, size_t *longeur);
void write_jpeg_header(FILE *file, int width, int height, bool is_pgm, int h1, int v1, int h2, int v2, int h3, int v3);

#endif // JPEG_WRITER_H