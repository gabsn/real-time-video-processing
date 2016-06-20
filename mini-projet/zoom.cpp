#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))

void ZOOM::zoom_with_latency() {
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

    /*****************************
    * Gestion de l'image de sortie
    ******************************/
    if (i_in > H4 && i_in <= H4+H2 && j_in == W4) { // On vient de récupérer une ligne de l'image que l'on veut afficher
        int i = i_in-(H4+1);
        for (unsigned int j=0; j<W2; j++) { // On remplit un carré de 4 pixels conjoints à la même valeur
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

void ZOOM::zoom() {
    //cout << "entrée dans la méthode zoom" << endl;
    if (!reset_n) {
        //cout << "resetn activated" << endl;
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
        //cout << "p_in_counter = " << p_in_counter << " ";
        //cout << "p_out_counter = " << p_out_counter << " " << endl;
        //cout << "i_in = " << i_in << " ";
        //cout << "j_in = " << j_in << " ";
        //cout << "i_out = " << i_out << " ";
        //cout << "j_out = " << j_out << endl;
        //cout << "-----------------------------------" << endl;
        
        if (h_in && (i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4)) { // On est dans la zone de l'image qui correspond à un zoom x2
            //cout << "W4 = " << W4 << endl;
            //cout << "W2+W4 = " << W2+W4 << endl;
            /******************************
             * Réception de l'image zoomée 
             ******************************/
            //cout << "Image active :" << endl;
            //cout << "p_in_counter = " << p_in_counter << " ";
            //cout << "p_out_counter = " << p_out_counter << " " << endl;
            //cout << "i_in = " << i_in << " ";
            //cout << "j_in = " << j_in << " ";
            //cout << "i_out = " << i_out << " ";
            //cout << "j_out = " << j_out << endl;
            //cout << "-----------------------------------" << endl;
 
            if (p_in_counter < SIZE) {
                image_in[p_in_counter++] = p_in;
                //cout << "p_in = " << (int)p_in << " & p_in_counter = " << p_in_counter << endl;

                if (i_in > H4 && j_in == W4) { // On vient de récupérer une ligne de l'image que l'on veut afficher
                    cout << "ligne récupérée" << endl;
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
                cout << "p_out = " << p_out << endl;
                h_out = true;
                v_out = (pixel_counter < 3*WIDTH) ? true : false;
            } else {
                p_out = 0;
                h_out = false;
                v_out = false;
            }
        } else { // On est en dehors de la zone de l'image qui nous intéresse
            //cout << "En dehors de la partie de l'image qui nous intéresse" << endl;
            p_in_counter++;
            p_out = 0;
            h_out = false;
            v_out = false;
        }
    }
}

