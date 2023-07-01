#include "../src/palette.h"
#include "../src/image.h"

int main(int argc, char **argv){
    palette pal;
    pal.OpenPalette(argv[1]);
    pal.display();
    image img("castor.jpg");
    return 0;
}