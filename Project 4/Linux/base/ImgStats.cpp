//
//  ImgStats.cpp
//
//
//  Created by Andrew Xue on 11/12/26.
//

#include "ImgStats.h"


using namespace img;

void Histogram( const ImgProc& in, float low, float high, float resolution, std::vector< std::vector<float> >& histogram ) {
    float* raw_pixels = in.raw();
    int channels = in.depth();
    long raw_size = (long)in.nx() * (long)in.ny() * (long)channels;
    
    float width = (high - low) / resolution;
    
    for (int channel = 0; channel < channels; channel++) {
        histogram.push_back( std::vector<float>(resolution,0.0) );
        
#pragma omp parallel for
        for (long i = channel; i < raw_size; i += channels) {
            int bin = int( (raw_pixels[i] - min) / width );
            if ( (bin >= 0) && (bin < resolution) ) {
                histogram[channel][bin]++;
            }
        }
    }
}


void Cumulative( const ImgProc& in, float low, float high, float resolution, std::vector< std::vector<float> >& cumulative ) {
    int channels = in.depth();
    
    std::vector< std::vector<float> > histogram;
    Histogram( in, low, high, resolution, histogram );
    
    
    for (int channel = 0; channel < channels; channel++) {
        std::vector<float> channel_cdf(resolution+1,0.0);
        float total = 0.0;
        
#pragma omp parallel for
        for (int bin = 0; bin < resolution; bin++) {
            total += histogram[channel][bin];
        }
        
#pragma omp parallel for
        for (int bin = 0; bin < resolution; bin++) {
            channel_cdf[bin+1] = histogram[channel][bin] / total;
            channel_cdf[bin+1] += channel_cdf[bin];
        }
        
        
        cumulative.push_back(channel_cdf);
    }
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

void ApplyCumulative( const ImgProc& in, float resolution, ImgProc& out ) {
    std::vector< std::vector<float> > cumulative;
    float* raw_pixels = in.raw();
    int channels = in.depth();
    long raw_size = (long)in.nx() * (long)in.ny() * (long)channels;
    float* img_data = new float[raw_size];
    
    float high = 1.0;
    float low = 0.0
    
    
    float width = (high - low) / resolution;
    Cumulative( in, low, high, resolution, std::vector< std::vector<float> >& cumulative );
    
    
    for (int channel = 0; channel < channels; channel++) {
#pragma omp parallel for
        for (long i = channel; i < raw_size; i += channels) {
            int bin = int( (raw_pixels[i] - low) / width );
            if ( (bin >= 0) && (bin < resolution) ) {
                img_data[i] = cumulative[channel][bin];
            }
        }
    }
    
    for( int y_image=0;y_image<in.ny();y_image++)
    {
#pragma omp parallel for
        for(int x_image=0;x_image<in.nx();x_image++)
        {
            std::vector<float> pixel(channels,0.0);
            for (int c = 0; c < channels; c++) {
                pixel[c] = img_data[c + Nc*(x_image + Nx*y_image)];
            }
            
            out.set_value(x_image, y_image, pixel);
        }
    }
}
