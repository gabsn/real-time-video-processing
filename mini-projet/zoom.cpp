#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))
#define W_MARGIN 154 

void ZOOM::reception() {
    while (1) {
        wait();
        if (!reset_n) {
            nb_p_tot = 0;
            nb_p_received = 0;
            nb_p_active = 0;
            i_out = 0;
            j_out = 0;
            restart = true;
            p_out = 0;
            h_out = false;
            v_out = false;
        } 

        i_in = nb_p_received / W;
        j_in = nb_p_received % W;

        /************************
         * Réception de l'image *
         ************************/
        nb_p_tot++;
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
        if (h_in && nb_p_received == W) {
            margin = nb_p_tot-nb_p_received;
            cout << "margin : " << margin << endl;
        }
<<<<<<< HEAD
=======
        if (ACTIVE_RANGE) {
            image_received[nb_p_active++] = p_in;
        }
>>>>>>> essai
    }
}

void ZOOM::envoi() {
    /***********************
     * Gestion des sorties *
     ***********************/
<<<<<<< HEAD
    if (i_out < H2 && j_out < W2) {
=======
    if (i_out < H2 && j_out >= 0 && j_out < W2) {
        p_out = image_received[i_out*W2+j_out];
>>>>>>> essai
        h_out = true;
        v_out = (i_out == 0 || (i_out == 1 && restart == true)) ? true : false;
        p_out = image_received[i_out*W2+j_out];
        wait();
    } else {
        h_out = false;
        v_out = false;
        p_out = 0;

<<<<<<< HEAD
=======
        i_out = (i_out == H2) ? 0 : i_out;
        j_out++;
>>>>>>> essai

        if (j_out == W2+W_MARGIN && restart) {
            j_out = 0;
            restart = false;
        } else if (j_out == W2+W_MARGIN && !restart) {
            i_out++;
            j_out = 0;
            restart = true;
        }
    }
}

