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
        pixel_counter = 0;
        p_in_counter = 0;
        p_out_counter = 0;
        i_in = 0;
        i_out = 0;
        j_in = 0;
        j_out = 0;

        image_in = new unsigned char[H2*W2];
        image_out = new unsigned char[SIZE];

        SC_METHOD(zoom);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void zoom();

    unsigned char * image_in;
    unsigned char * image_out;
    unsigned int pixel_counter;
    unsigned int p_in_counter;
    unsigned int p_out_counter;
    unsigned int i_in;
    unsigned int j_in;
    unsigned int i_out;
    unsigned int j_out;
};

#endif

