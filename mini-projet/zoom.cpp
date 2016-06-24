#include "zoom.h"

#define ACTIVE_RANGE ((i_in >= H4) && (i_in < H2+H4) && (j_in >= W4) && (j_in < W2+W4))
#define W_MARGIN 154 
#define H_MARGIN 49 
#define WB 874
#define HB 625
#define SIZE_B 824*625

void ZOOM::receiving() {
    if (!reset_n) {
        nb_p_received = 0;
        nb_p_out = 0;
    } 

    i_in = nb_p_received / W;
    j_in = nb_p_received % W;

    if (h_in && ((v_in && nb_p_received < 3*W) || (!v_in && nb_p_received >= 3*W))) {
        nb_p_received = (nb_p_received == SIZE-1) ? 0 : nb_p_received+1;
        nb_p_out = (nb_p_out == W2*H2-1) ? 0 : nb_p_out+1;

        if (ACTIVE_RANGE) {
            image_received[(i_in-H2)*W2+(j_in-W2)] = p_in;
            start_sending = true;
        }
    }
}

void ZOOM::sending() {
    while (1) {
        if (!reset_n) {
            p_out = 0;
            h_out = false;
            v_out = false;
        }

        wait();
        if (start_sending) {

            

    i_out = nb_p_out / W2;
    j_out = nb_p_out % W2;
    if (
    if (new_image && i_out < H2 && j_out < W2) {
        p_out = image_received[i_out*W2+j_out];
        h_out = true;
        v_out = (i_out == 0 || (i_out == 1 && restart == true)) ? true : false;

        j_out = (j_tot % 2 != 0) ? j_out + 1 : j_out; 
    } else if (new_image && i_out < H2 && j_out >= W2) {
        p_out = 0;
        h_out = false;
        v_out = false;

        j_out++;

        if (j_out == W2+W_MARGIN && restart) {
            j_out = 0;
            restart = false;
        } else if (j_out == W2+W_MARGIN && !restart) {
            i_out++;
            j_out = 0;
            restart = true;
        }
    } else if (new_image && i_out == H2) {
        p_out = 0;
        h_out = false;
        v_out = false;

        i_out = 0;
        j_out = 0;
        restart = true;
        new_image = false;
    } else {
        p_out = 0;
        h_out = false;
        v_out = false;
    }

    //cout << "i_out : " << i_out << " & j_out : " << j_out << endl;
}

