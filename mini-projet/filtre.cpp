#include "filtre.h"

void FILTRE::receiving() {
    if (!reset_n) {
        nb_p_received = 0;
        start_sending = false;
    }

    i_in = nb_p_received / W;
    j_in = nb_p_received % W;

    if (h_in && ((v_in && nb_p_received < 3*W) || (!v_in && nb_p_received >= 3*W))) {
        if (nb_p_received < SIZE-1) {
            image_received[i_in % R][j_in] = p_in;
            nb_p_received++;
        } else {
            nb_p_received = 0;
            start_sending = false;
        }

        // Si on a au moins reçu la première ligne et les deux pixels suivant 
        if (nb_p_received >= W+2)
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
    double matrix[R][R];
    double result = 0;

    // On copie la matrice de convolution du filtre pour pouvoir la modifier
    for (int l=0; l<R; l++) {
        for (int c=0; c<R; c++) {
            matrix[l][c] = coeff[l][c];
        }
    }
    
    // On gère les bords par "effet miroir"
    if (j == 0) {
        for (int l=0; l<R; l++) {
            matrix[l][0] = 0;
            matrix[l][2] *= 2;
        }
    } else if (j == W-1) {
        for (int l=0; l<R; l++) {
            matrix[l][0] *= 2;
            matrix[l][2] = 0;
        }
    }
    if (i == 0) {
        for (int c=0; c<R; c++) {
            matrix[0][c] = 0;
            matrix[2][c] *= 2;
        }
    } else if (i == H-1) {
        for (int c=0; c<R; c++) {
            matrix[0][c] *= 2;
            matrix[2][c] = 0;
        }
    }

    // On applique la convolution 
    for (int l=0; l<R; ++l) {
        for (int c=0; c<R; ++c) {
            if (matrix[l][c] != 0) {
                result += matrix[l][c]*image_received[(i-1+l) % R][j-1+c];
            }
        }
    }
    result /= 9;

    return (unsigned char)result;
}

