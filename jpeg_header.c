#include "../include/jpeg_header.h"
#include "../include/qtables.h"

// écris un octet:
void write_octet(FILE *file, uint8_t octet) {
    fputc(octet,file);
}

//écris un marqueur sur deux octets : 
void write_markeur(FILE *file, uint16_t markeur) {
    write_octet(file,(markeur>>8) & 0xFF);
    write_octet(file,markeur & 0xFF);         
}

//START OF IMAGE
void write_soi(FILE *file){
    write_markeur(file,0xFFD8);
}

//END OF IMAGE
void write_eoi(FILE *file) {
    write_markeur(file,0xFFD9);
}


void write_app0_header(FILE *file) {
    write_markeur(file,0xFFE0);
    fputc(0x00,file);
    fputc(0x10,file); // Length of APP0, including length itself
    const char JFIF_ID[] = "JFIF";  // cette methode n'ignore pas le '\0'.
    fwrite(JFIF_ID, sizeof(JFIF_ID),1,file);  // avant on avait un prblm avec fputc !
    fputc(0x01,file); fputc(0x01,file); // Version 1.1
    for (int i=0;i<7;i++){
        fputc(0x00, file);
    }
}
void write_DQT_SECTION(FILE *file){
    write_markeur(file,0xFFDB);
    fputc(0x00,file);
    fputc(0x84,file); //Longueur 
    write_DQT_Y(file);
    write_DQT_CbCr(file);
}
void write_DQT_Y(FILE *file) {
    fputc(0x00, file); // Precision (0 for 8-bit)
    for (int i=0;i< 64;i++) {
        fputc(quantification_table_Y[i],file);
    }
}
void write_DQT_CbCr(FILE *file) {
    fputc(0x01, file); // Precision (1 for table 1)
    for (int i=0; i< 64;i++) {
        fputc(quantification_table_CbCr[i],file);
    }
}


void write_DHT_section(FILE *file, uint8_t type, uint8_t index, uint8_t *nbr_lengths, uint8_t *symbols, uint16_t total_symbols) {
    write_markeur(file,0xFFC4);
    // 2 octets pour la longueur, 1 octet pour type/index, 16 pour les nombres de symboles, reste pour les symboles
    uint16_t length =2 +1 +16 +total_symbols; 
    fputc(length >> 8,file);
    fputc(length & 0xFF,file);
    uint8_t table_info = (type << 4) | index;
    fputc(table_info, file);

    // Nbr de symboles avec des code de longueur 1 à 16
    for (int i = 0; i < 16; i++) {
        fputc(nbr_lengths[i], file);
    }

    // Table des symboles, triés par longueur
    for (int i = 0; i < total_symbols; i++) {
        fputc(symbols[i], file);
    }
}

void write_dht(FILE *file) {
    // Écriture des tables DC pour Y, Cb, et Cr
    write_DHT_section(file, 0, 0, htables_nb_symb_per_lengths[0][0], htables_symbols[0][0], htables_nb_symbols[0][0]); // DC, Y
    write_DHT_section(file, 0, 1, htables_nb_symb_per_lengths[0][1], htables_symbols[0][1], htables_nb_symbols[0][1]); // DC, CbCr

    // Écriture des tables AC pour Y, Cb, et Cr
    write_DHT_section(file, 1, 0, htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0]); // AC, Y
    write_DHT_section(file, 1, 1, htables_nb_symb_per_lengths[1][1], htables_symbols[1][1], htables_nb_symbols[1][1]); // AC, CbCr
}
void write_sof0_gris(FILE *file, int width, int height) {
    write_markeur(file, 0xFFC0);
    int length = 11;
    fputc(length>>8 , file);
    fputc(length & 0xFF, file);
    // Précision en bits par composante(8 pour le mode baseline)
    fputc(8, file);
    fputc(height>>8, file);
    fputc(height& 0xFF, file);
    fputc(width>>8, file);
    fputc(width & 0XFF, file);
    fputc(1, file);
    // Informations pour la composante Y
    fputc(0, file); // Identifiant
    fputc((1<< 4) |1,file); // Facteur d'échantillonnage : 1x1
    fputc(0,file); // Indice de la table de quantification
}
//NOTA BENE :
//le nombre de composantes de couleur utilisées (1 en niveaux de gris, 3 en YCbCr)
void write_sof0_couleur(FILE *file, int width, int height,int h1, int v1, int h2, int v2, int h3, int v3) {
    write_markeur(file, 0xFFC0);
    int length = 8 + 3 * 3;// 3 composantes (Y, Cb, Cr)
    fputc(length >> 8, file);
    fputc(length & 0xFF, file);
    // Précision en bits par composante(8 pour baseline)
    fputc(8, file);
    fputc(height >> 8, file);
    fputc(height & 0xFF, file);
    fputc(width >> 8, file);
    fputc(width & 0xFF, file);
    // Nombre de composantes (3 pour YCbCr)
    fputc(3, file);
    // Informations pour la composante Y
    fputc(1, file); // Identifiant
    fputc((h1<< 4) |v1,file); // Facteur d'échantillonnage : h1xv1
    fputc(0,file); // Indice de la table de quantification

    // Informations pour la composante Cb
    fputc(2, file); 
    fputc((h2<<4) |v2,file); 
    fputc(1,file); 

    // Informations pour la composante Cr
    fputc(3, file); 
    fputc((h3<<4) |v3,file);
    fputc(1,file); 
}


void write_sos_couleur(FILE *file) {
    write_markeur(file, 0xFFDA);
    uint16_t longueur = 12;
    fputc( longueur >>8, file);
    fputc(longueur & 0xFF, file);
    //nbr de composantes :
    fputc(3, file);
    // Informations sur la composante
    for (int i = 0; i < 3; i++) {
        // ID de la composante (1 = Y, 2 = Cb, 3 = Cr)
        fputc(i + 1, file);
        //selecteur de la table de huffmann (DC and AC)
        if (i == 0) {  // Y component
            fputc((0 << 4) | 0, file);  // DC table 0, AC table 0
        } else {  // Cb or Cr component
            fputc((1 << 4) | 1, file);  // DC table 1, AC table 1
        }
    }
    fputc(0, file);   //  ss
    fputc(63, file);  //  se
    fputc(0, file);   //(0 pour AH ET AL en mode baseline)
}

void write_sos_gris(FILE *file) {
    write_markeur(file, 0xFFDA);
    uint16_t longueur = 8;
    fputc(longueur >>8, file);
    fputc(longueur & 0xFF, file);
    fputc(1, file);
    fputc( 0, file);
    fputc((0 << 4) | 0, file);  // DC table 0, AC table 0
    fputc(0, file);   
    fputc(63, file);
    fputc(0, file);
}

// Byte stuffing pour éviter la confusion avec les marqueurs
void write_octet_avec_stuffing(FILE *file, unsigned char octet) {
    fputc(octet, file);
    if (octet == 0xFF) {
        fputc(0x00, file); 
    }
}

unsigned char convertir_bitstring_en_octet(const char *bitstring) {
    unsigned char octet = 0;
    for (int i =0; i< 8;i++) {
        octet =(octet << 1) | (bitstring[i]=='1');
    }
    return octet;
}

void traiter_et_ecrire_octet(FILE *fichier, const char *bitstream, size_t offset) {
    unsigned char octet = convertir_bitstring_en_octet(bitstream+offset);
    write_octet_avec_stuffing(fichier, octet);
}

void traiter_et_ecrire_bits_restants(FILE *fichier, const char *flux_bits, size_t offset, size_t bits_restants) {
    char dernier_octet[8] = { '0', '0', '0', '0', '0', '0', '0', '0' };
    for (size_t j = 0; j < bits_restants; j++) {
        dernier_octet[j] = flux_bits[offset + j];
    }
    unsigned char octet = convertir_bitstring_en_octet(dernier_octet);
    write_octet_avec_stuffing(fichier, octet);
}

void write_data_encode_avec_stuffing(FILE *file, const char *bitstream, size_t length) {
    size_t i = 0;
    for (; i + 8 <= length; i += 8) {
        traiter_et_ecrire_octet(file, bitstream, i);
    }

    // bits restants si la longueur n'est pas un multiple de 8
    if (i < length) {
        traiter_et_ecrire_bits_restants(file, bitstream, i, length - i);
    }
}


size_t calcul_longueur_bitstream(const char *data) {
    size_t longueur = 0;
    for (size_t i = 0; data[i] != '\0'; i++) {
        if (data[i] != ' ') {
            longueur++;
        }
    }
    return longueur;
}

//on remplit le bitstream sans espaces
void remplir_bitstream(const char *data, char *bitstream) {
    size_t j = 0;
    for (size_t i = 0; data[i] != '\0'; i++) {
        if (data[i] != ' ') {
            bitstream[j++] = data[i];
        }
    }
    bitstream[j] = '\0';
}

char* generate_bitstream(const char *data, size_t *longueur) {
    *longueur = calcul_longueur_bitstream(data);
    char *bitstream = (char *)malloc(*longueur + 1);
    remplir_bitstream(data, bitstream);
    return bitstream;
}

void write_jpeg_file(const char *filename, int width, int height, bool is_pgm, int h1, int v1, int h2, int v2, int h3, int v3, const char *data, size_t *longueur) {
    FILE *file = fopen(filename, "wb");
    write_jpeg_header(file,width,height,is_pgm,h1, v1, h2,v2,h3,v3);
    char *bitstream = generate_bitstream(data,longueur);
    write_data_encode_avec_stuffing(file,bitstream,*longueur);
    free(bitstream);
    write_eoi(file);
    fclose(file);
}

void write_jpeg_header(FILE *file, int width, int height, bool is_pgm, int h1, int v1, int h2, int v2, int h3, int v3) {
    write_soi(file);
    write_app0_header(file);
    write_DQT_SECTION(file);
    if (is_pgm) {
        write_sof0_gris(file, width, height);
        write_dht(file);
        write_sos_gris(file);
    } else {
        write_sof0_couleur(file, width, height, h1, v1, h2, v2, h3, v3);
        write_dht(file);
        write_sos_couleur(file);
    }
}
