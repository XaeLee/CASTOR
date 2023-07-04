#include "engine.h"
#include <iostream>

using namespace std;

engine::engine(string filename){
    this->original = QImage(filename.c_str());
    this->color_count = -1;
}

engine::~engine(){

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
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

void engine::ReduceColors(int n, int algotype){
    switch (algotype)
    {
    case MEDIAN_CUT:
        this->ReduceColorsMEDIAN(n);
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
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}