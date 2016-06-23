#ifndef ZOOM_H
#define ZOOM_H

#include <systemc.h>
#include "image.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(ZOOM) {

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
        nb_p_received  = 0;
        nb_p_active    = 0;
        i_image_out    = 0;
        count          = 0;
        total_count    = 0;
        image_received = new unsigned char[H2*W2];
        image_out      = new unsigned char[SIZE];

        SC_METHOD(zoom);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void zoom();

    bool new_image;
    unsigned int nb_p_received;
    unsigned int nb_p_active;
    unsigned int i_image_out;
    unsigned int count;
    unsigned int total_count;
    unsigned char * image_received;
    unsigned char * image_out;
};

#endif

