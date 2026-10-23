//
//  Fractals.cpp
//
//
//  Created by Andrew Xue on 10/22/26.
//

#include "Fractals.h"

using namespace img;

ColorLUT::ColorLUT() :
gamma (1.0)
{}

void ColorLUT::setColors(std::vector< std::vector<float> >& newBands) {
    bands = {};
    for (int i = 0; i < int(newBands.size()); i++) {
        bands.push_back(newBands[i]);
    }
}

void ColorLUT::operator()(const double& value, std::vector<float>& C) const {
    if (value <= 1) {
        double s = value * bands.size();
        int m = int(s);
        int m_prime = m + 1;
        double omega = s - m;
        
        if (m_prime > int(bands.size())) {
            m_prime = m;
        }
        
        for (int i = 0; i < 3; i++) {
            C[i] = bands[m][i] * (1 - omega) + bands[m_prime][i] * omega;
        }
    }
}


JuliaSet::JuliaSet(const Point& P0, const int nb, const int cycle) {
    c = P0;
    nb_iterations = nb;
    cycles = cycle;
}

Point JuliaSet::operator()(const Point& P) const {
    std::complex<long double> Pc(P.x,P.y);
    std::complex<long double> cc(c.x,c.y);
    for(int i=0;i<nb_iterations;i++)
    {
        std::complex<long double> temp = Pc;
        for(int c=1;c<cycles;c++)
        {
            temp = temp * Pc;
        }
        Pc = temp + cc;
    }
    Point Pout;
    Pout.x = (double)Pc.real();
    Pout.y = (double)Pc.imag();
    return Pout;
}

void JuliaSet::ApplyJuliaWarpLUT( const Point& center, const double range,
                                   const Warp& w, const ColorLUT& lut, ImgProc& out ) {
    fractal_t R = 2.0;
    for (int j = 0; j < out.ny(); j++) {
#pragma omp parallel for
        for (int i = 0; i < out.nx(); i++) {
            Point P;
            P.x = 2.0*(fractal_t)i/(fractal_t)out.nx() - 1.0;
            P.y = 2.0*(fractal_t)j/(fractal_t)out.ny() - 1.0;
            P.x *= range;
            P.y *= range;
            P.x += center.x;
            P.y += center.y;
            Point PP = w(P); // iterations of zN+1 = zN^q + zc
            fractal_t rate = std::sqrt(PP.x*PP.x + PP.y*PP.y)/R;
            std::vector<float> vs(3,0.0);
            lut((double)rate,vs);
            out.set_value(i,j, vs );
        }
    }
}
