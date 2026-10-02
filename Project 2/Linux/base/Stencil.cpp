//
//  Stencil.cpp
//  
//
//  Created by Andrew Xue on 10/1/26.
//

#include <OpenImageIO/imageio.h>

#include "Stencil.h"

using namespace img;

Stencil::Stencil(int halfwidth = 1) {
    half_width = halfwidth;
    stencil_size = (2*halfwidth+1) * (2*halfwidth+1);
    
    //stencil_values follows leftmost value == [0], next value in row == [1], and so on
    //access specific value by using x + y * (2*halfwidth+1)
    //default size will be 9 values
    stencil_values = std::unique_ptr<float[]>(new float[stencil_size]);
    
    randomizeStencil();
}

Stencil::~Stencil() {
    if (stencil_values != 0) { delete[] stencil_values; stencil_values = 0; }
}

void Stencil::randomizeStencil() {
    float stencil_total = 0;
    std::uniform_real_distribution<> ran_value(-0.1, 0.1);
#pragma omp parallel for
    for (int i = 0; i < stencil_size; i++) {
        stencil_values[i] = ran_value();
        stencil_total += stencil_values[i];
    }
    
    int center = std::ceil( (2*halfwidth+1) / 2 );
    stencil_value[center + center*(2*halfwidth+1)] += (1 - stencil_total);
}

float& Stencil::operator()(int i, int j) {
    return stencil_values[i + j*(2*halfwidth+1)];
}

const float& Stencil::operator()(int i, int j) const {
    return stencil_values[i + j*(2*halfwidth+1)];
}


void BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out ) {
    out.clear( in.nx(), in.ny(), in.depth() );
    for( int j=0;j<out.ny();j++)
    {
        int jmin = j - stencil.halfwidth();
        int jmax = j + stencil.halfwidth();
#pragma omp parallel for
        for(int i=0;i<out.nx();i++)
        {
            int imin = i - stencil.halfwidth();
            int imax = i + stencil.halfwidth();
            std::vector<float> pixel(out.depth(),0.0);
            std::vector<float> sample(in.depth(),0.0);
            for(int jj=jmin;jj<=jmax;jj++)
            {
                int stencilj = jj-j;
                int jjj = jj;
                if(jjj < 0 ){ jjj += out.ny(); }
                if(jjj >= out.ny() ){ jjj -= out.ny(); }
                for(int ii=imin;ii<=imax;ii++)
                {
                    int stencili = ii-i;
                    int iii = ii;
                    if(iii < 0 ){ iii += out.nx(); }
                    if(iii >= out.nx() ){ iii -= out.nx(); }
                    const float& stencil_value = stencil(stencili, stencilj);
                    in.value(iii,jjj,sample);
                    for(size_t c=0;c<sample.size();c++){ pixel[c] += sample[c] * stencil_value; }
                }
            }
            out.set_value(i,j,pixel);
        }
    }
}
