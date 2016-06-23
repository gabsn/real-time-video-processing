#include <cstdio>
#include <sstream>
#include <iomanip>
#include <string>
#include "video_out.h"

using namespace std;

void VIDEO_OUT::writer() {
    if(reset_n == false) {
        pixel_counter = 0;
        image_counter = 0;
    } 
    
    if ((href && (vref && pixel_counter < 3*W)) || (href && (!vref && pixel_counter >= 3*W))) {
        if (pixel_counter < SIZE)
            image->pixel[pixel_counter++] = pixel_in;
        else
            cout << "ERROR: pixel_counter > SIZE" << endl;

        if (pixel_counter == SIZE) {
            ostringstream convert;
            convert << image_counter;
            string number = convert.str(); 
            string filename = "wallace_out_"+number;
            image_write(image,filename.c_str());
            pixel_counter = 0;
            image_counter++;
        }
    }
}
