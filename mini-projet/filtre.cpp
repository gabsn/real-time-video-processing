#include "filtre.h"

#define R 3

void FILTRE::receiving() {
    if (!reset_n) {
        nb_p_received = 0;
        start_sending = false;
    }

    if (h_in && ((v_in && nb_p_received < 3*W) || (!v_in && nb_p_received >= 3*W))) {
        if (nb_p_received < SIZE-1) {
            image_received[nb_p_received++] = p_in;
        } else {
            nb_p_received = 0;
            start_sending = false;
        }

        if (nb_p_received >= W+1)
            start_sending = true;
    }
}

void FILTRE::sending() {
    while (1) {
        if (!reset_n) {
            p_out = 0;
            h_out = false;
            v_out = false;
        }

        // On attend pour se synchroniser
        while (!start_sending) wait();

        for(int i=0; i<625; i++) {
            for(int j=0; j<874; j++) {
                // on attend le prochain coup d'horloge
                wait();
                // Si on est dans la fenêtre active, on sort le pixel courant
                // Rappel : une trame vidéo fait 874*625, l'image active est de 720*576
                if((i<H) && (j<W))
                    p_out = gen_pix(i,j);
                else
                    p_out = 0;

                // Génération de HREF
                // HREF est actif pendant la sortie des pixels actifs
                h_out = (i<H) && (j<W);

                // Génération de VREF
                // VREF est actif pendant les 3 premières lignes d'une image
                v_out = (i<3);
            }
        }
    }
}

unsigned char FILTRE::gen_pix(int i, int j) {
    double result = 0;
    int coeff[R][R] = {{1,1,1}
                      ,{1,1,1}
                      ,{1,1,1}
                      };

    // On calcule le "poid" de la matrice de convolution
    unsigned int poid = 0;
    for (int l=0; l<R; l++)
        for (int c=0; c<R; c++)
            poid += coeff[l][c];
    if (poid == 0) poid = 1;
    
    // On gère les bords par "effet miroir"
    if (j == 0) {
        for (int l=0; l<R; l++) {
            coeff[l][0] = 0;
            coeff[l][2] *= 2;
        }
    } else if (j == W-1) {
        for (int l=0; l<R; l++) {
            coeff[l][0] *= 2;
            coeff[l][2] = 0;
        }
    }
    if (i == 0) {
        for (int c=0; c<R; c++) {
            coeff[0][c] = 0;
            coeff[2][c] *= 2;
        }
    } else if (i == H-1) {
        for (int c=0; c<R; c++) {
            coeff[0][c] *= 2;
            coeff[2][c] = 0;
        }
    }

    // On applique la convolution 
    for (int l=0; l<R; ++l) {
        for (int c=0; c<R; ++c) {
            if (coeff[l][c] != 0) {
                result += coeff[l][c]*image_received[(i-1+l)*W+(j-1+c)];
            }
        }
    }
    result /= poid;

    return (unsigned char)result;
}

