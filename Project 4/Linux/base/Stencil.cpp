//
//  Stencil.cpp
//  
//
//  Created by Andrew Xue on 10/1/26.
//

#include <OpenImageIO/imageio.h>

#include "Stencil.h"

using namespace img;

Stencil::Stencil() {
    half_width = 1;
    stencil_size = (2*half_width+1) * (2*half_width+1);
    
    stencil_values = 0;
}

Stencil::Stencil(int halfwidth) {
    half_width = halfwidth;
    stencil_size = (2*half_width+1) * (2*half_width+1);
    
    //stencil_values follows leftmost value == [0], next value in row == [1], and so on
    //access specific value by using x + y * (2*halfwidth+1)
    //default size will be 9 values
    stencil_values = new float[stencil_size];
    
    randomizeStencil();
}

Stencil::~Stencil() {
    if (stencil_values != 0) { delete[] stencil_values; stencil_values = 0; }
}

void Stencil::randomizeStencil() {
    float stencil_total = 0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> ran_value(-0.1, 0.1);
#pragma omp parallel for
    for (int i = 0; i < stencil_size; i++) {
        stencil_values[i] = ran_value(gen);
        stencil_total += stencil_values[i];
    }
    
    int center = std::ceil( (2*half_width+1) / 2 );
    stencil_values[center + center*(2*half_width+1)] += (1 - stencil_total);
}

//stencil values are centered around the center value so adjustment is needed
float& Stencil::operator()(int i, int j) {
    return stencil_values[(i+half_width) + (j+half_width)*(2*half_width+1)];
}

const float& Stencil::operator()(int i, int j) const {
    return stencil_values[(i+half_width) + (j+half_width)*(2*half_width+1)];
}



void Stencil::BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out ) {
    out.clear( in.nx(), in.ny(), in.depth() );
    for( int y_image=0;y_image<out.ny();y_image++)
    {
        int y_image_min = y_image - stencil.halfwidth();
        int y_image_max = y_image + stencil.halfwidth();
#pragma omp parallel for
        for(int x_image=0;x_image<out.nx();x_image++)
        {
            int x_image_min = x_image - stencil.halfwidth();
            int x_image_max = x_image + stencil.halfwidth();
            std::vector<float> pixel(out.depth(),0.0);
            std::vector<float> sample(in.depth(),0.0);
            for(int y_stencil=y_image_min;y_stencil<=y_image_max;y_stencil++)
            {
                int stencilj = y_stencil-y_image;
                for(int x_stencil=x_image_min;x_stencil<=x_image_max;x_stencil++)
                {
                    int stencili = x_stencil-x_image;
                    const float& stencil_value = stencil(stencili, stencilj);
                    if (y_stencil >= 0 || x_stencil >= 0) {
                        if (y_stencil < out.ny() || x_stencil < out.nx()) {
                            in.value(x_stencil,y_stencil,sample);
                        }
                    }
                    for(size_t c=0;c<sample.size();c++){ pixel[c] += sample[c] * stencil_value; }
                }
            }
            out.set_value(x_image,y_image,pixel);
        }
    }
}
