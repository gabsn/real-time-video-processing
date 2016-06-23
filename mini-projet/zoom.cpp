#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))

void ZOOM::zoom() {
    if (!reset_n) {
        nb_p_received = 0;
        nb_p_active = 0;
        i_image_out = 0;
        p_out = 0;
        h_out = false;
        v_out = false;
    } 

    unsigned int i_in, j_in, i, j;
    i_in = nb_p_received / W;
    j_in = nb_p_received % W;
    i = i_in-H4;
    j = j_in-W4;

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

    /*******************************
    * Gestion de l'image de sortie *
    ********************************/
    if (ACTIVE_RANGE) {
        // On remplit un carré de 4 pixels conjoints à la même valeur (Vérifié)
        image_out[2*i*W+2*j] = image_received[i*W2+j];
        image_out[2*i*W+2*j+1] = image_received[i*W2+j];
        image_out[(2*i+1)*W+2*j] = image_received[i*W2+j];
        image_out[(2*i+1)*W+2*j+1] = image_received[i*W2+j];
    } 

    /***********************
     * Gestion des sorties *
     ***********************/
    if (i_image_out < SIZE) {
        p_out = image_out[i_image_out++];
        h_out = true;
        v_out = (i_image_out < 3*W) ? true : false;
    } else if (i_image_out == SIZE) {
        i_image_out = 0;
        p_out = 0;
        h_out = false;
        v_out = false;
    } else {
        p_out = 0;
        h_out = false;
        v_out = false;
    }
}

