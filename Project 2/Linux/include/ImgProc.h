//
//  ImgProc.h
//  
//
//  Created by Andrew Xue on 9/10/26.
//

#ifndef IMG_PROC_H
#define IMG_PROC_H

#include <vector>
#include <string>

using namespace std;

namespace img{
    
    class ImgProc
    {
    public:
        ImgProc();
        ~ImgProc();
        
        void clear();
        void clear(int nX, int nY, int nC);
        
        int nx() const { return Nx; }
        int ny() const { return Ny; }
        int depth() const { return Nc; }
        
        void load( const std::string &filename );
        void write( const std::string &filename ) const;
        
        void value( int i, int j, std::vector<float>& pixel ) const;
        void set_value( int i, int j, const std::vector<float>& pixel );
        
        ImgProc( const ImgProc& img ); // copy constructor
        ImgProc& operator=(const ImgProc& img ); // copy assignment
        
        float* raw(){ return img_data; }
    
    private:
        int Nx, Ny, Nc;
        long Nsize;
        float* img_data;
    };
    
}

#endif 
