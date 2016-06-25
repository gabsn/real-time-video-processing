#ifndef FILTRE_H
#define FILTRE_H

#include <systemc.h>
#include "image.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(FILTRE) {
    unsigned int nb_p_received;
    sc_signal<bool> start_sending;

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
        image_received = new unsigned char[SIZE];
        nb_p_received = 0;
        start_sending = false;

        SC_METHOD(receiving);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);

        SC_CTHREAD(sending,clk.pos());
        async_reset_signal_is(reset_n,false);
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:
    void receiving();
    void sending();
    unsigned char gen_pix(int,int);

    unsigned char * image_received;
};

#endif

