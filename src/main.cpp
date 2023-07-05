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
    engine img("examples/castor.jpg");

    //p = img.ExtractPalette(7, OCTREE);
    img.ReduceColors(255, OCTREE, BASIC);
    //p = img.getPalette();
    //cout << "palette has " << to_string(p.getColorCount()) << " color.s" << endl;

    //p.display();
    img.saveEdit("output/castorOCTREE255.jpg");
    return 0;
}