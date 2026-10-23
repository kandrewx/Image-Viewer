//
//  Fractals.h
//
//
//  Created by Andrew Xue on 10/22/26.
//

#ifndef FRACTALS_H
#define FRACTALS_H

#include "warp.h"
#include <complex>

namespace img
{




class ColorLUT
{
public:
    
    ColorLUT(double gamma = 1.0);
    ~ColorLUT(){}
    
    void setColors(std::vector< std::vector<float> > newBands) { bands = newBands; }
    
    // Generate color value from the input value
    void operator()(const double& value, std::vector<float>& C) const;
    
private:
    
    double gamma;
    std::vector<float> black;
    std::vector< std::vector<float> > bands;
};


void ApplyFractalWarpLUT( const Point& center, const double range, const Warp& w, const ColorLUT& lut, ImgProc& out );


class JuliaSet : public Warp
{
public:
    
    JuliaSet(){};
    JuliaSet(const Point& P0, const int nb, const int cycle);
    ~JuliaSet(){}
    
    Point operator()(const Point& P) const;
    
private:
    
    Point c;
    int nb_iterations;
    int cycles;
    
};




}

#endif
