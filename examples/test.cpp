#include "../src/palette.h"

int main(int argc, char **argv){
    palette pal;
    pal.OpenPalette(argv[1]);
    pal.display();
    return 0;
}