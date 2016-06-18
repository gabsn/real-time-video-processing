#include "filtre.h"

void FILTRE::average() {
    if (!reset_n) {
        pixel_received_i = 0;
        pixel_computed_i = 0;
        p_out = 0;
        h_out = false;
        v_out = false;
    } else if (h_in && pixel_received_i < WIDTH+1 && v_in) { // Première ligne de pixels + premier pixel de la première cologne
        image[pixel_received_i++] = p_in;
        p_out = 0;
        h_out = false;
        v_out = false;
    } else if (h_in && pixel_received_i >= WIDTH+1) { // On peut calculer un p_out => h_out = true;
        image[pixel_received_i] = p_in; 
        if (pixel_received_i == SIZE-1) { // On a reçu le dernier pixel de l'image
            pixel_received_i = 0;
        } else if (pixel_computed_i == SIZE-1) {
            pixel_computed_i = 0;
        } else if (pixel_received_i < SIZE-1 && pixel_received_i >= pixel_computed_i) {
            pixel_computed_i = pixel_received_i-(WIDTH+1);
            pixel_received_i++;
        } else if (pixel_computed_i < SIZE-1 && pixel_received_i < pixel_computed_i) {
            pixel_computed_i++;
            pixel_received_i++;
        } 

        if (pixel_computed_i == 0) // pixel en haut à gauche
        { 
            p_out = (image[0] + image[1] + image[WIDTH] + image[WIDTH+1])/4; 
        } 
        else if (pixel_computed_i == WIDTH-1) // pixel en haut à droite
        {
            p_out = (image[WIDTH-2] + image[WIDTH-1] + image[2*WIDTH-2] + image[2*WIDTH-1])/4;
        } 
        else if (pixel_computed_i == (HEIGHT-1)*WIDTH) // pixel en bas à gauche
        {
            p_out = (image[(HEIGHT-2)*WIDTH] + image[(HEIGHT-2)*WIDTH+1] + image[(HEIGHT-1)*WIDTH] + image[(HEIGHT-1)*WIDTH+1])/4;
        }
        else if (pixel_computed_i == SIZE-1) // pixel en bas à droite
        {
            p_out = (image[(HEIGHT-2)*WIDTH-2] + image[(HEIGHT-1)*WIDTH-1] + image[HEIGHT*WIDTH-2] + image[HEIGHT*WIDTH-1])/4;
        }
        else if (pixel_computed_i < WIDTH-1) // bord haut
        {
            p_out = (image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1] + image[pixel_computed_i+WIDTH])/4;
        }
        else if (pixel_computed_i % WIDTH == 0) // bord gauche
        {
            int d = pixel_computed_i / WIDTH;
            p_out = (image[(d-1)*WIDTH] + image[pixel_computed_i] + image[pixel_computed_i+1] + image[(d+1)*WIDTH])/4;
        }
        else if (pixel_computed_i % WIDTH == WIDTH-1) // bord droit
        {
            int d = pixel_computed_i / WIDTH;
            p_out = (image[(d-1)*WIDTH] + image[pixel_computed_i] + image[pixel_computed_i-1] + image[(d+1)*WIDTH])/4;
        }
        else if (pixel_computed_i >= (HEIGHT-1)*WIDTH) // bord bas
        {
            int d = pixel_computed_i / WIDTH;
            p_out = (image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1] + image[(d-1)*WIDTH])/4;
        }
        else // intérieur de l'image
        {
            int d = pixel_computed_i / WIDTH;
            p_out = ( image[(d-1)*WIDTH-1] + image[(d-1)*WIDTH] + image[(d-1)*WIDTH+1] 
                    + image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1]
                    + image[(d+1)*WIDTH-1] + image[(d+1)*WIDTH] + image[(d+1)*WIDTH+1] 
                    )/9; 
        }

        h_out = true;
        if (pixel_computed_i < 3*WIDTH)
            v_out = true;
        else 
            v_out = false;
    } else {
        p_out = 0;
        h_out = false;
        v_out = false;
    }
}

