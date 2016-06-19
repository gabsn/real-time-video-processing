#include "zoom.h"

void ZOOM::zoom() {
    if (!reset_n) {
        pixel_counter = 0;
        p_in_counter = 0;
        p_out_counter = 0;
        i_in = 0;
        i_out = 0;
        j_in = 0;
        j_out = 0;
        p_out = 0;
        h_out = false;
        v_out = false;
    } else {
        i_in = p_in_counter / WIDTH;
        j_in = p_in_counter % WIDTH;
        i_out = p_out_counter / W2;
        j_out = p_out_counter % W2;
        
        if (h_in && i_in >= H4 && i_in < H2+H4 && j_in >= W4 && j_in < W2+W4) { // On est dans la zone de l'image qui correspond à un zoom x2
            /******************************
             * Réception de l'image zoomée 
             ******************************/
            if (p_in_counter < SIZE) {
                image_in[p_in_counter++] = p_in;

                if (i_in > H4 && j_in == 0) { // On vient de récupérer une ligne de l'image que l'on veut afficher
                    for (unsigned int j=0; j<W2; j+=2) { // On remplit un carré de 4 pixels conjoints à la même valeur
                        image_out[i_out*WIDTH+j] = image_in[i_out*W2+j/2];
                        image_out[i_out*WIDTH+j+1] = image_in[i_out*W2+j/2];
                        image_out[(i_out+1)*WIDTH+j] = image_in[i_out*W2+j/2];
                        image_out[(i_out+1)*WIDTH+j+1] = image_in[i_out*W2+j/2];
                    }
                    p_out_counter = p_out_counter + 2*WIDTH;
                }
            }

            /**************************
             * Envoi de l'image zoomée
             **************************/
            if (i_in > H4) {
                p_out = image_out[pixel_counter++];
                h_out = true;
                v_out = (pixel_counter < 3*WIDTH) ? true : false;
            } else {
                p_out = 0;
                h_out = false;
                v_out = false;
            }
        } else {
            p_out = 0;
            h_out = false;
            v_out = false;
        }
    }
}

