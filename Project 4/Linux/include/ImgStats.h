//
//  ImgStats.h
//
//
//  Created by Andrew Xue on 11/12/26.
//

#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include "ImgProc.h"
#include <vector>
#include <string>
#include <cmath>


namespace img
{

void Histogram( const ImgProc& in, std::vector<float>& low, std::vector<float>& high, float resolution, std::vector< std::vector<float> >& histogram );


void Cumulative( const ImgProc& in, std::vector<float>& low, std::vector<float>& high, float resolution, std::vector< std::vector<float> >& cumulative );


void Statistics( const ImgProc& in, std::vector<float>& max, std::vector<float>& min, std::vector<float>& ave, std::vector<float>& stddev );

void ContrastConvert( const ImgProc& in, std::vector<float>& ave, std::vector<float>& stddev, ImgProc& out );

void ApplyCumulative( const ImgProc& in, float resolution, ImgProc& out );

}
#endif
