#include "filtre.h"

void FILTRE::gen_p_out() {
    int d = pixel_computed_i / W;
    int r = pixel_computed_i % W;

    if (pixel_computed_i == 0) // pixel en haut à gauche
    { 
        p_out = (image[0] + image[1] + image[W] + image[W+1])/4; 
    } 
    else if (pixel_computed_i == W-1) // pixel en haut à droite
    {
        p_out = (image[W-2] + image[W-1] + image[2*W-2] + image[2*W-1])/4;
    } 
    else if (pixel_computed_i == (H-1)*W) // pixel en bas à gauche
    {
        p_out = (image[(H-2)*W] + image[(H-2)*W+1] + image[(H-1)*W] + image[(H-1)*W+1])/4;
    }
    else if (pixel_computed_i == SIZE-1) // pixel en bas à droite
    {
        p_out = (image[(H-2)*W-2] + image[(H-1)*W-1] + image[H*W-2] + image[H*W-1])/4;
    }
    else if (pixel_computed_i < W-1) // bord haut
    {
        p_out = ( image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1]
                + image[(d+1)*W-1+r] + image[(d+1)*W+r] + image[(d+1)*W+1+r] 
                )/6; 
    }
    else if (pixel_computed_i % W == 0) // bord gauche
    {
        p_out = ( image[(d-1)*W] + image[(d-1)*W+1]
                + image[pixel_computed_i] + image[pixel_computed_i+1]
                + image[(d+1)*W] + image[(d+1)*W+1]
                )/6; 
    }
    else if (pixel_computed_i % W == W-1) // bord droit
    {
        p_out = ( image[(d-1)*W+r-1] + image[(d-1)*W+r] 
                + image[pixel_computed_i-1] + image[pixel_computed_i] 
                + image[(d+1)*W+r-1] + image[(d+1)*W+r]
                )/6;
    }
    else if (pixel_computed_i >= (H-1)*W) // bord bas
    {
        p_out = ( image[(d-1)*W+r-1] + image[(d-1)*W+r] + image[(d-1)*W+r+1] 
                + image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1] 
                )/6;
    }
    else // intérieur de l'image
    {
        p_out = ( image[(d-1)*W-1+r] + image[(d-1)*W+r] + image[(d-1)*W+1+r] 
                + image[pixel_computed_i-1] + image[pixel_computed_i] + image[pixel_computed_i+1]
                + image[(d+1)*W-1+r] + image[(d+1)*W+r] + image[(d+1)*W+1+r] 
                )/9; 
        //cout << "initial_pixel(" << pixel_computed_i << ") = " << (int)image[pixel_computed_i] << " & p_out = " << (int)p_out <<  " : " << endl;
        //cout << (int)image[(d-1)*W-1] << " " << (int)image[(d-1)*W] << " " << (int)image[(d-1)*W+1] << " " << endl 
        //     << (int)image[pixel_computed_i-1] << " " << (int)image[pixel_computed_i] << " " << (int)image[pixel_computed_i+1] << " " << endl
        //     << (int)image[(d+1)*W-1] << " " << (int)image[(d+1)*W] << " " << (int)image[(d+1)*W+1] << " " << endl;
    }
}

void FILTRE::average() {
    if (!reset_n) {
        pixel_received_i = 0;
        pixel_computed_i = 0;
        p_out = 0;
        h_out = false;
        v_out = false;
    } else if (h_in && pixel_received_i < W+1 && v_in) { // Première ligne de pixels + premier pixel de la première cologne
        image[pixel_received_i++] = p_in;
        p_out = 0;
        h_out = false;
        v_out = false;
    } else if (h_in && pixel_received_i >= W+1) { // On calcule p_out tout en recevant des pixels
        image[pixel_received_i] = p_in; 
        gen_p_out();
        h_out = true;
        // Gestion du signal v_ref
        if (pixel_computed_i < 3*W)
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

