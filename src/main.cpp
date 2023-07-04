#include "palette.h"
#include "engine.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    
    if (argc < 2){
        cout << "Expected file to read palette from" << endl;
        return -1;
    }
    palette p;
    p = p.OpenPalette(argv[1]);
    p.display();
    engine img("examples/castor.jpg");
    img.ReduceColors(6, MEDIAN_CUT);
    //p = img.ExtractPalette(15, MEDIAN_CUT);
    img.AdaptToPaletteClosest(p, DI_FLOYD_STEINBERG);
    img.saveEdit("output/castorMypalFLOYD.jpg");
    return 0;
}