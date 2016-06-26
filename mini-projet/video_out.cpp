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
        if (pixel_counter < SIZE-1)
            image->pixel[pixel_counter++] = pixel_in;
        else {
            image->pixel[pixel_counter] = pixel_in;
            pixel_counter = 0;

            // Ecriture du fichier .png
            ostringstream convert;
            convert << image_counter;
            string number = convert.str(); 
            string filename = "wallace_out_"+number;
            image_write(image,filename.c_str());
            image_counter++;
        }
    }
}
