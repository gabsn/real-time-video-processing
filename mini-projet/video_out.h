#ifndef VIDEO_OUT_H
#define VIDEO_OUT_H

#include <systemc.h>
#include "image.h"

#define SIZE 720*576

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(VIDEO_OUT) {

    // IO PORTS
    sc_in<bool>          clk;
    sc_in<bool>          reset_n;
    sc_in<unsigned char> pixel_in;
    sc_in<bool>          href;
    sc_in<bool>          vref;

    /***************************************************
     *  constructeur
     **************************************************/
    SC_CTOR(VIDEO_OUT) {
        image            = new Image();
        image->width     = 720;
        image->height     = 576;
        image->pixel     = new unsigned char[SIZE];
        pixel_counter    = 0;
        image_counter    = 0;

        SC_METHOD(writer);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    /***************************************************
     *  méthodes et champs internes
     **************************************************/
    private:

    void writer();

    Image * image;
    unsigned int pixel_counter;
    unsigned int image_counter;
};

#endif

