#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))

void ZOOM::receiving() {
    if (!reset_n) {
        nb_p_received = 0;
        big_width = W;
        big_width_set = false;
        start_sending = false;
    } 

    i_in = nb_p_received / W;
    j_in = nb_p_received % W;

    if (!h_in && nb_p_received == W && !big_width_set) {
        big_width = big_width+1;
    } else if (h_in && nb_p_received == W && !big_width_set) {
        big_width_set = true;
    }

    if (h_in && ((v_in && nb_p_received < 3*W) || (!v_in && nb_p_received >= 3*W))) {
        nb_p_received = (nb_p_received == SIZE-1) ? 0 : nb_p_received+1;

        if (ACTIVE_RANGE) {
            image_received[(i_in-H4) % (H4+1)][j_in-W4] = p_in;
            start_sending = true;
        }
    }
}

void ZOOM::sending() {
    while (1) {
        if (!reset_n) {
            p_out = 0;
            h_out = false;
            v_out = false;
        }

        // On attend pour se synchroniser
        while (!start_sending) wait();

        for(unsigned int i=0; i<625; i++) {
            for(unsigned int j=0; j<big_width; j++) {
                // on attend le prochain coup d'horloge
                wait();
                // Si on est dans la fenêtre active, on sort le pixel courant
                // Rappel : une trame vidéo fait 874*625, l'image active est de 720*576
                if((i<H) && (j<W))
                    p_out = image_received[i/2 % (H4+1)][j/2];
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

