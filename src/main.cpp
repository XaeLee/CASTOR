#include "palette.h"
#include "engine.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){

/*    
    if (argc < 2){
        cout << "Expected file to read palette from" << endl;
        return -1;
    }
    srand(time(NULL));*/
    palette p;
    //p = p.OpenPalette(argv[1]);
    //p.display();
    engine img("examples/testCube.jpg");

    img.ReduceColors(4, MEDIAN_CUT);
    p = img.ExtractPalette(4, MEDIAN_CUT);

    img.AdaptToPaletteClosest(p, DI_JARVISJN);
    img.saveEdit("output/doublepass.jpg");
    return 0;
}