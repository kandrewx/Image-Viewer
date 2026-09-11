//
//  ImgProc.c
//
//
//  Created by Andrew Xue on 9/10/26.
//

#include <OpenImageIO/imageio.h>

#include "ImgProc.h"

using namespace img;

ImgProc::ImgProc() :
Nx (0),
Ny (0),
Nc (0),
Nsize (0),
img_data (0)
{}
ImgProc::~ImgProc() { clear(); }

//cleaning up the image
void ImgProc::clear() {
    if (img_data != 0) { delete[] img_data; img_data = 0; }
    Nx = 0;
    Ny = 0;
    Nc = 0;
    Nsize = 0;
}
void ImgProc::clear(int nX, int nY, int nC) {
    clear();
    Nx = nX;
    Ny = nY;
    Nc = nC;
    Nsize = (long)Nx * (long)Ny * (long)Nc;
    img_data = new float[Nsize];
#pragma omp parallel for
    for (long i = 0; i < Nsize; i++) { img_data[i] = 0.0; }
}

//creating the image
void ImgProc::load( const std::string &filename ) {
    auto inp = OIIO::ImageInput::open (filename);
    const OIIO::ImageSpec &spec = inp->spec();
    
    clear(spec.width, spec.height, spec.nchannels);
    
    auto pixels = std::unique_ptr<float[]>(new float[Nx * Ny * Nc]);
    inp->read_image(0, 0, 0, Nc, OIIO::TypeDesc::FLOAT, &pixels[0]);
    
#pragma omp parallel for
    for (int i = 0; i < Nsize; i++) {
        img_data[i] = pixels[i];
    }
    
    inp->close();
}

//turning the image into a file
void ImgProc::write( const std::string &filename ) const {
    std::unique_ptr<OIIO::ImageOutput> out = OIIO::ImageOutput::create(filename);
    if (!out)
        return;  // error
    OIIO::ImageSpec spec(Nx, Ny, Nc, OIIO::TypeDesc::FLOAT);
    out->open(filename, spec);
    out->write_image(OIIO::TypeDesc::FLOAT, img_data);
    out->close();
}

void ImgProc::value( int i, int j, std::vector<float>& pixel ) const {
    pixel.clear();
    if ( img_data == 0 ) { return; }
    if ( i<0 || i>=Nx ) { return; }
    if ( j<0 || j>=Ny ) { return; }
    pixel.resize(Nc);
    for( int c=0;c<Nc;c++ ) {
        pixel[c] = img_data[c + Nc*(i + Nx*j)];
    }
    return;
}
void ImgProc::set_value( int i, int j, const std::vector<float>& pixel ) {
    if ( img_data == 0 ) { return; }
    if ( i<0 || i>=Nx ) { return; }
    if ( j<0 || j>=Ny ) { return; }
    if ( Nc > (int)pixel.size() ) { return; } // do nothing if pixel does not have sufficient number of channels
    for ( int c=0;c<Nc;c++ ) {
        img_data[c + Nc*(i + Nx*j)] = pixel[c];
    }
    return;
}

ImgProc::ImgProc( const img::ImgProc& v ) : // copy constructor
Nx (v.Nx),
Ny (v.Ny),
Nc (v.Nc),
Nsize (v.Nsize)
{
    img_data = new float[Nsize];
#pragma omp parallel for
    for (long i = 0; i < Nsize; i++) { img_data[i] = v.img_data[i]; }
}

ImgProc& ImgProc::operator=(const ImgProc& v ) { // copy assignment
    if (this == &v) { return *this; }
    if (Nx != v.Nx || Ny != v.Ny || Nc != v.Nc) {
        clear();
        Nx = v.Nx;
        Ny = v.Ny;
        Nc = v.Nc;
        Nsize = v.Nsize;
        img_data = new float[Nsize];
    }
#pragma omp parallel for
    for (long i = 0; i < Nsize; i++) { img_data[i] = v.img_data[i]; }
    return *this;
}
