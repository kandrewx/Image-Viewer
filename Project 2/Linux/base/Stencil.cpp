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
