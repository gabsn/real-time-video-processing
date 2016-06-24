#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))
#define W_MARGIN 154 
#define H_MARGIN 49 
#define WB 874
#define HB 625
#define SIZE_B 824*625

void ZOOM::receiving() {
    if (!reset_n) {
        nb_p_received = 0;
        nb_p_out = 0;
        start_sending = false;
    } 

    i_in = nb_p_received / W;
    j_in = nb_p_received % W;

    if (h_in && ((v_in && nb_p_received < 3*W) || (!v_in && nb_p_received >= 3*W))) {
        nb_p_received = (nb_p_received == SIZE-1) ? 0 : nb_p_received+1;
        nb_p_out = (nb_p_out == W2*H2-1) ? 0 : nb_p_out+1;

        if (ACTIVE_RANGE) {
            image_received[(i_in-H4)*W2+(j_in-W4)] = p_in;
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

        for(int i=0; i<625; i++) {
            for(int j=0; j<874; j++) {
                // on attend le prochain coup d'horloge
                wait();
                // Si on est dans la fenêtre active, on sort le pixel courant
                // Rappel : une trame vidéo fait 874*625, l'image active est de 720*576
                if((i<H) && (j<W))
                    p_out = image_received[i/2*W2+j/2];
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

