include "filtre.h"

void FILTRE::average() {
    if (!reset_n) {
        pixel_received_i = 0;
        pixel_computed_i = 0;
    } else if (h_in && pixel_received_i < WIDTH+1) { // Première ligne de pixels + première pixel de la première cologne
        image[pixel_received_i++] = p_in;
    } else if (h_in) { 
        image[pixel_received_i++] = p_in;
        pixel_computed_i = pixel_received_i-(WIDTH+2);
        unsigned char pixel = image[pixel_computed_i];

        if (pixel_computed_i == 0) // pixel en haut à gauche
        { 
            p_out = (image[0] + image[1] + image[WIDTH] + image[WIDTH+1])/4; 
        } 
        else if (pixel_computed == WIDTH-1) // pixel en haut à droite
        {
            p_out = (image[WIDTH-2] + image[WIDTH-1] + image[2*WIDTH-2] + image[2*WIDTH-1])/4;
        } 
        else if (pixel_computed == (HEIGHT-1)*WIDTH) // pixel en bas à gauche
        {
            p_out = (image[(HEIGHT-2)*WIDTH] + image[(HEIGHT-2)*WIDTH+1] + image[(HEIGHT-1)*WIDTH] + image[(HEIGHT-1)*WIDTH+1])/4;
        }
        else if (pixel_computed == HEIGHT*WIDTH-1) // pixel en bas à droite
        {
            p_out = (image[(HEIGHT-2)*WIDTH-2] + image[(HEIGHT-1)*WIDTH-1] + image[HEIGHT*WIDTH-2] + image[HEIGHT*WIDTH-1])/4;
        }




        
    }
    h_out = h_in;
    v_out = v_in;
}

