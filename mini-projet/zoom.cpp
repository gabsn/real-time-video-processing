#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))
#define W_MARGIN 154 

void ZOOM::zoom() {
    if (!reset_n) {
        nb_p_received = 0;
        nb_p_active = 0;
        i_out = 0;
        j_out = 0;
        restart = true;
        p_out = 0;
        h_out = false;
        v_out = false;
    } 

    unsigned int i_in, j_in;
    i_in = nb_p_received / W;
    j_in = nb_p_received % W;

    /************************
     * Réception de l'image *
     ************************/
    if ((h_in && v_in && nb_p_received < 3*W) || (h_in && !v_in && nb_p_received >= 3*W)) {
        if (nb_p_received < SIZE) {
            nb_p_received++;
        } else {
            nb_p_received = 0;
            nb_p_active = 0;
        }
        if (ACTIVE_RANGE)
            image_received[nb_p_active++] = p_in;
    }

    /***********************
     * Gestion des sorties *
     ***********************/
    if (i_out < H2 && j_out < W2) {
        p_out = image_received[i_out*W2+j_out];
        h_out = true;
        v_out = (i_out == 0 || (i_out == 1 && restart == true)) ? true : false;

        j_out = (j_in % 2 != 0) ? j_out + 1 : j_out; 
    } else {
        p_out = 0;
        h_out = false;
        v_out = false;

        i_out = (i_out == H2) ? 0 : i_out;

        if (j_out >= W2 && j_out < W2+W_MARGIN-1) {
            j_out++; // on est dans la zone de marge
        } else if (j_out == W2+W_MARGIN-1 && restart) {
            j_out = 0;
            restart = false;
        } else if (j_out == W2+W_MARGIN-1 && !restart) {
            i_out++;
            j_out = 0;
            restart = true;
        }
    }
}

