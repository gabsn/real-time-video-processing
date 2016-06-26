#ifndef FILTRE_H
#define FILTRE_H

#include <systemc.h>
#include "image.h"

#define R 3

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
    SC_HAS_PROCESS(FILTRE);
    FILTRE(sc_module_name _name, double _coeff[][R]) : sc_module(_name) {
        nb_p_received = 0;
        start_sending = false;
        for (int l=0; l<R; ++l)
            for (int c=0; c<R; ++c) {
                coeff[l][c] = _coeff[l][c];
                cout << coeff[l][c] << " ";
            }
        cout << endl;

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

    unsigned char image_received[R][W];
    double coeff[R][R];
};

#endif

