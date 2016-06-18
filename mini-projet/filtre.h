#ifndef FILTRE_H
#define FILTRE_H

#include <systemc.h>
#include "image.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(FILTRE) {

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
    SC_CTOR(FILTRE) {
        pixel_received_i = 0;
        pixel_computed_i = 0;
        image = new unsigned char[SIZE];

        SC_METHOD(average);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void average();

    unsigned char * image;
    unsigned int pixel_received_i;
    unsigned int pixel_computed_i;
};

#endif

