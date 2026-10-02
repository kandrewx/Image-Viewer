//
//  Stencil.h
//
//
//  Created by Andrew Xue on 10/1/26.
//

#ifndef STENCIL_H
#define STENCIL_H

#include <tuple>
#include "ImgProc.h"

using namespace std;

namespace img{
    
    class Stencil
    {
    public:
        Stencil(int halfwidth);
        ~Stencil();
        int halfwidth() const { return half_width; }
        float& operator()(int i, int j) const;
    
    private:
        int half_width;
        float *stencil_values;
    };
    
}

#endif
