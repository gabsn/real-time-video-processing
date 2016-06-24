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
    sc_signal<bool> start_sending;
    bool no_image_received;
    unsigned int nb_p_received;
    unsigned int nb_p_tot;
    unsigned int nb_p_out;
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
        start_sending = false;
        no_image_received = true;
        nb_p_received  = 0;
        nb_p_active    = 0;
        nb_p_out    = 0;
        nb_p_tot    = 0;
        i_out = 0;
        j_out = 0;
        i_image_out    = 0;
        count          = 0;
        total_count    = 0;
        image_received = new unsigned char[H2*W2];
        image_out      = new unsigned char[SIZE];

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

    unsigned int i_image_out;
    unsigned int count;
    unsigned int total_count;
    unsigned char * image_received;
    unsigned char * image_out;
};

#endif

