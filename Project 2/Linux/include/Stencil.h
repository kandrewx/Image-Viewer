//
//  Stencil.h
//
//
//  Created by Andrew Xue on 10/1/26.
//

#ifndef STENCIL_H
#define STENCIL_H

#include <random>
#include <cmath>
#include "ImgProc.h"

using namespace std;

namespace img{
    
    class Stencil
    {
    public:
        Stencil();
        Stencil(int halfwidth);
        ~Stencil();
        int halfwidth() const { return half_width; }
        void randomizeStencil();
        float& operator()(int i, int j);
        const float& operator()(int i, int j) const;
        
        void BoundedLinearConvolution( const Stencil& stencil, const ImgProc& in, ImgProc& out );

    
    private:
        int half_width;
        int stencil_size;
        float *stencil_values;
    };

}

#endif
