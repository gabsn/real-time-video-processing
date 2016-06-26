#ifndef ZOOM_H
#define ZOOM_H

#include <systemc.h>
#include "image.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(ZOOM) {
    // Public attributes
    sc_signal<bool> start_sending;
    unsigned int nb_p_received;
    unsigned int i_in;
    unsigned int j_in;

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
        start_sending = false;
        nb_p_received  = 0;

        SC_METHOD(receiving);
        sensitive << clk.pos();

        SC_CTHREAD(sending,clk.pos());
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void receiving();
    void sending();

    unsigned char image_received[H4+1][W2];
};

#endif

