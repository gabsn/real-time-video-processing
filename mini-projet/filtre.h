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
    sc_ out<bool>         v_out;

    /***************************************************
     *  constructeur
     **************************************************/
    SC_CTOR(FILTRE) {
        line_number;
        lines = new unsigned char[3*WIDTH];

        SC_METHOD(average);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void average();

    unsigned char * lines;
    unsigned int line_number;
};

#endif

