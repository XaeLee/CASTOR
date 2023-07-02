#include "engine.h"
#include <iostream>

using namespace std;

int color_distance(QRgb p1, QRgb p2){
    int r1 = qRed(p1);
    int g1 = qGreen(p1);
    int b1 = qBlue(p1);

    int r2 = qRed(p2);
    int g2 = qGreen(p2);
    int b2 = qBlue(p2);

    return abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2);
}

QRgb match(QRgb base, palette p){
    int index = 0;
    int current_distance = INT_MAX;
    vector<QRgb> cols = p.getColors();
    for (int i = 0; i<cols.size(); i++){
        int dist = color_distance(base, cols.at(i));
        if (dist < current_distance){
            current_distance = dist;
            index = i;
        }
    }
    return cols[index];
}

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
        this->ExtractPaletteMEDIAN(n);
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

void engine::AdaptToPaletteClosest(palette p, int n, int algotype){
    switch (algotype)
    {
    case MEDIAN_CUT:
        this->AdaptToPaletteClosestMEDIAN(p, n);
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}