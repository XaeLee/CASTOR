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
    img.AdaptToPaletteClosest(p, p.getColorCount(), MEDIAN_CUT);
    img.saveEdit("output/castorMypal.jpg");
    return 0;
}