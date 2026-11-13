//
//  ImgStats.cpp
//
//
//  Created by Andrew Xue on 11/12/26.
//

#include "ImgStats.h"


using namespace img;

void Histogram( const ImgProc& in, float low, float high, float resolution, std::vector< std::vector<float> >& histogram ) {
    
}


void Cumulative( const ImgProc& in, float low, float high, float resolution, std::vector< std::vector<float> >& cumulative ) {
    
}


void Statistics( const ImgProc& in, std::vector<float>& max, std::vector<float>& min, std::vector<float>& ave, std::vector<float>& stddev ) {
    
    float* raw_pixels = in.raw();
    int channels = in.depth();
    long raw_size = (long)in.nx() * (long)in.ny() * (long)channels;
    int img_size = in.nx() * in.ny();
    
    
    for (int channel = 0; channel < channels; channel++) {
        max.push_back(0.0);
        min.push_back(0.0);
        ave.push_back(0.0);
        
#pragma omp parallel for
        for (long i = channel; i < raw_size; i += channels) {
            ave[channel] += raw_pixels[i];
            
            if (max[channel] < raw_pixels[i]) {
                max[channel] = raw_pixels[i];
            }
            if (min[channel] > raw_pixels[i]) {
                min[channel] > raw_pixels[i];
            }
        }
        
    }
    
    for (int channel = 0; channel < channels; channel++) {
        double rms = 0;
        
        ave[channel] /= (in.nx() + in.ny());
        
#pragma omp parallel for
        for (int i = channel; i < raw_size; i += channels) {
            rms += (raw_pixels[i] - ave[channel]) * (raw_pixels[i] - ave[channel]);
        }
        
        rms = std::sqrt( rms/img_size );
        stddev.push_back(rms);
    }
}

bool ApplyCumulative( const ImgProc& in, float resolution, ImgProc& out ) {
    return true;
}
