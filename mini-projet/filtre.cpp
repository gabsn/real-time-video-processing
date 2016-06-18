#include "filtre.h"

void FILTRE::gen_p_out() {
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
        //cout << "d = " << d << endl;
        p_out = ( image[(d-1)*WIDTH-1] + image[(d-1)*WIDTH] + image[(d-1)*WIDTH+1] 
                + image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1]
                + image[(d+1)*WIDTH-1] + image[(d+1)*WIDTH] + image[(d+1)*WIDTH+1] 
                )/9; 
        //cout << "initial_pixel(" << pixel_computed_i << ") = " << (int)image[pixel_computed_i] << " & p_out = " << (int)p_out <<  " : " << endl;
        //cout << (int)image[(d-1)*WIDTH-1] << " " << (int)image[(d-1)*WIDTH] << " " << (int)image[(d-1)*WIDTH+1] << " " << endl 
        //     << (int)image[pixel_computed_i-1] << " " << (int)image[pixel_computed_i] << " " << (int)image[pixel_computed_i+1] << " " << endl
        //     << (int)image[(d+1)*WIDTH-1] << " " << (int)image[(d+1)*WIDTH] << " " << (int)image[(d+1)*WIDTH+1] << " " << endl;
    }
}

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
    } else if (h_in && pixel_received_i >= WIDTH+1) { // On calcule p_out tout en recevant des pixels
        image[pixel_received_i] = p_in; 
        gen_p_out();
        h_out = true;
        // Gestion du signal v_ref
        if (pixel_computed_i < 3*WIDTH)
            v_out = true;
        else 
            v_out = false;

        // Gestion des indices du pixel reçu et du pixel calculé
        if (pixel_received_i < SIZE-1) {
            pixel_received_i++;
            pixel_computed_i++;
        } else if (pixel_received_i == SIZE-1) {
            pixel_received_i = 0;
            pixel_computed_i++;
        }
    } else if (!h_in && pixel_received_i == 0 && pixel_computed_i != 0) { // On a tous les pixels de l'image active en notre possession
        gen_p_out(); 
        h_out = true;
        v_out = false;
        if (pixel_computed_i < SIZE-1) {
            pixel_computed_i++;
        } else if (pixel_computed_i == SIZE-1) {
            pixel_computed_i = 0;
        }
    } else {
        p_out = 0;
        h_out = false;
        v_out = false;
    }
}

