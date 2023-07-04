#include "palette.h"
#include "engine.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    
    if (argc < 2){
        cout << "Expected file to read palette from" << endl;
        return -1;
    }
    srand(time(NULL));
    palette p;
    p = p.OpenPalette(argv[1]);
    p.display();
    engine img("examples/smallgradient.jpg");
    img.ReduceColors(15, MEDIAN_CUT);
    p = img.ExtractPalette(15, MEDIAN_CUT);
    img.AdaptToPaletteClosest(p, DI_NOISE);
    img.saveEdit("output/gradientNOISE15.jpg");
    return 0;
}