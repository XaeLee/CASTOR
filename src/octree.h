#include <QtGui/QImage>
#include <vector>

#include "palette.h"

using namespace mypalette;

class ocnode
{
public:
    ocnode *children[8];
    int childrenCount = 0;

    float r_sum = 0;
    float g_sum = 0;
    float b_sum = 0;

    int passes = 0; //nb of times we passed here

    int pixel_count = 0;
    ocnode(/* args */);

    ~ocnode();
};

class octree
{
private:
    void mergeLeast(ocnode *n, int min);
    void FillPalette(ocnode *n);
    void addColor(ocnode *n, QRgb color, int level);
    void setColor(ocnode *n, QRgb color);
    void writeColor(ocnode *n);

    ocnode *root;
    int currcolorsCount = 0;
    int wantedColors = 0;
    int maxDepth = 4;

    unsigned int MinReferenceCount;
    void getMinRefCount(ocnode *n);


    palette p;

public:

    // creates an octree that will reduce to n colors
    octree(int n);
    int getColorsCount() {return currcolorsCount;};
    palette reduceColors(QImage img);

    ~octree();
};