#include "../include/dct.h"

double cos_stocke[N][N];
double coefficient_C[N];

        //------------dct optimisée -----------------
// on commence par prépcalculer  les constantes

void prepcalcul_cste() {
    for (int x =0;x<N;x++) {
        coefficient_C[x] = (x==0)?(1.0/sqrt(2.0)):1.0;
        for (int i=0;i<N;i++) {
            cos_stocke[x][i]=cos((2*x+1)*i*PI/(2.0*N));
        }
    }
}

// on exécute la DCT une ligne ou une colonne en tenant compte des symétries
void applique_dct_unidimentionnel(const double *input, double *output) {
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j =0; j <N; j++) {
            sum += input[j]*cos_stocke[j][i];
        }
        output[i] = sum*coefficient_C[i];
    }
}

// ici, on utilise la technique butterfly 
void dct(int **block, int **dctBlock) {
    double dct_en_cours[N][N];
    double lign_result[N];
    double col_result[N];

    // d'abord on retranche 128 à chaque pixel
    for (int x = 0; x < N; x++) {
        for (int y = 0;y <N;y++) {
            dct_en_cours[x][y]=block[x][y]-128;
        }
    }

    // applique_dct_unidimentionnel sur les lignes
    for (int x = 0; x < N; x++) {
        applique_dct_unidimentionnel(dct_en_cours[x],lign_result);
        for (int i=0; i< N;i++) {
            dct_en_cours[x][i] =lign_result[i];
        }
    }

    // applique_dct_unidimentionnel sur les colonnes
    for (int y = 0;y<N;y++) {
        for (int x = 0;x<N;x++) {
            col_result[x]=dct_en_cours[x][y];
        }
        applique_dct_unidimentionnel(col_result,lign_result);
        for (int x=0;x<N;x++) {
            dctBlock[x][y] = (int)(lign_result[x]/4);
        }
    }
}
