#include "engine.h"
#include <iostream>

using namespace std;

engine::engine(string filename){
    this->original = QImage(filename.c_str());
    this->color_count = -1;
    this->edited = QImage(this->original.width(), this->original.height(), this->original.format());
}

engine::engine(){
    this->color_count = 0;
}

engine::engine(QImage img){
    this->original = img;
    this->color_count = -1;
    this->edited = QImage(this->original.width(), this->original.height(), this->original.format());
}

engine::~engine(){

}

void engine::openImage(QImage img){
    this->original = img;
    this->color_count = -1;
    this->edited = QImage(this->original.width(), this->original.height(), this->original.format());
}

void engine::saveEdit(string filename){
    this->edited.save(filename.c_str());
}

palette engine::ExtractPalette(int n, int algotype){
    switch (algotype)
    {
    case MEDIAN_CUT:
        return this->ExtractPaletteMEDIAN(n);
        break;
    case OCTREE:
        return this->ExtractPaletteOctree(n);
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

void engine::ReduceColors(int n, int algotype, int matchType){
    switch (algotype)
    {
    case MEDIAN_CUT:
        this->ReduceColorsMEDIAN(n, matchType);
        this->usedalgotype = algotype;
        break;
    case OCTREE:
        this->ReduceColorsOctree(n, matchType);
        this->usedalgotype = algotype;
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

void engine::AdaptToPaletteClosest(palette p, int matchType){
    switch (matchType)
    {
    case BASIC:
        this->AdaptToPaletteClosestNeutral(p);
        break;
    case DI_FLOYD_STEINBERG:
        this->AdaptToPaletteClosestFloyd_Steinberg(p);
        break;
    case DI_NOISE:
        this->AdaptToPaletteClosestNoise(p);
        break;
    case DI_JARVISJN:
        this->AdaptToPaletteClosestJarvis(p);
        break;
    case DI_ATKINSON:
        this->AdaptToPaletteClosestAtkinson(p);
        break;
    case DI_SIERRA:
        this->AdaptToPaletteClosestSierra(p);
        break;
    case DI_SIERRA_TWO_ROW:
        this->AdaptToPaletteClosestSierraTwoRows(p);
        break;
    case DI_SIERRA_LITE:
        this->AdaptToPaletteClosestSierraLite(p);
        break;
    case matchType::DI_BAYER_4X4:
        this->AdaptToPaletteClosestBayer4x4(p);
        break;
    case matchType::DI_BAYER_8x8:
        this->AdaptToPaletteClosestBayer8x8(p);
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

void engine::ReduceColorsOctree(int n, int matchType){
    octree t(n);
    this->ediPal = t.reduceColors(this->original);
    cout << "done with reducting" << endl;
    this->AdaptToPaletteClosest(ediPal, matchType);    
}

palette engine::ExtractPaletteOctree(int n){
    octree t(n);
    return t.reduceColors(this->original);
}