#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))

void ZOOM::zoom() {
    if (!reset_n) {
        nb_p_received = 0;
        nb_p_active = 0;
        new_line = false;
        i_image_out = 0;
        line_counter = W2;
        p_out = 0;
        h_out = false;
        v_out = false;
    } 

    i_in = nb_p_received / WIDTH;
    j_in = nb_p_received % WIDTH;

    /***********************
     * Réception de l'image
     ***********************/
    if (h_in && ACTIVE_RANGE) {
        image_received[nb_p_active++] = p_in;
        nb_p_received++;
    } else if (nb_p_received == SIZE) {
        nb_p_received = 0;
        nb_p_active = 0;
    } else if (h_in) {
        nb_p_received++;
    }

    /******************************
    * Gestion de l'image de sortie
    *******************************/
    if (i_in > H4 && i_in <= H4+H2 && j_in == W4) { // On vient de récupérer une ligne de l'image que l'on veut afficher
        int i = i_in-(H4+1);
        for (unsigned int j=0; j<W2; j++) { // On remplit un carré de 4 pixels conjoints à la même valeur (Vérifié)
            image_out[2*i*WIDTH+2*j] = image_received[i*W2+j];
            image_out[2*i*WIDTH+2*j+1] = image_received[i*W2+j];
            image_out[(2*i+1)*WIDTH+2*j] = image_received[i*W2+j];
            image_out[(2*i+1)*WIDTH+2*j+1] = image_received[i*W2+j];
        }
        new_line = true;
    }

    /**********************
     * Gestion des sorties
     **********************/
    if (new_line && (i_image_out < SIZE)) {
        //cout << "(new_line && (i_image_out < SIZE)) = true : line_counter = " << line_counter << endl;
        if (line_counter > 0) {
            line_counter--;
        } else {
            new_line = false;
            line_counter = W2;
        }
        p_out = image_out[i_image_out++];
        h_out = true;
        v_out = (i_image_out < 3*WIDTH) ? true : false;
    } else if (i_image_out == SIZE) {
        new_line = false;
        line_counter = W2;
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

