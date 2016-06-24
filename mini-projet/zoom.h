#ifndef ZOOM_H
#define ZOOM_H

#include <systemc.h>
#include "image.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(ZOOM) {
    bool new_image;
    bool restart;
    unsigned int nb_p_received;
    unsigned int nb_p_tot;
    unsigned int nb_p_active;
    unsigned int margin;
    unsigned int i_in;
    unsigned int j_in;
    unsigned int i_tot;
    unsigned int j_tot;
    unsigned int i_out;
    unsigned int j_out;

    // IO PORTS
    sc_in<bool>          clk;
    sc_in<bool>          reset_n;
    sc_in<unsigned char> p_in;
    sc_in<bool>          h_in;
    sc_in<bool>          v_in;

    sc_out<unsigned char> p_out;
    sc_out<bool>          h_out;
    sc_out<bool>          v_out;

    /***************************************************
     *  constructeur
     **************************************************/
    SC_CTOR(ZOOM) {
        new_image = false;
        restart = true;
        nb_p_received  = 0;
        nb_p_active    = 0;
        nb_p_tot    = 0;
        i_out = 0;
        j_out = 0;
        i_image_out    = 0;
        count          = 0;
        total_count    = 0;
        image_received = new unsigned char[H2*W2];
        image_out      = new unsigned char[SIZE];

        SC_METHOD(zoom);
        sensitive << clk.pos();
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void zoom();

    unsigned int i_image_out;
    unsigned int count;
    unsigned int total_count;
    unsigned char * image_received;
    unsigned char * image_out;
};

#endif

