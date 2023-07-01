#pragma once

#include <string>

#include <qt5/QtGui/qimage.h>

#include "palette.h"

using namespace std;
class image
{
private:
    //original loaded image
    QImage original;
    //edited image
    QImage edited;
    //number of colors in edited image, -1 if never edited
    int color_count;
    //palette in use for edited
    palette origiPal;

public:
    /**
     * extracts color palette from original image with n colors.
     * n must be > 1
     * if n == color_count, returns origiPal.
     * else, processes KMeans on original to extract colors - does NOT edit edited.
    */
    palette ExtractPalette(int n);
    
    /**
     * Processes a KMeans on original image, writes edited as the output.
     * n must be > 1
     * changes color_count to n, rewrites origiPal.
    */
    void ReduceColors(int n);

    /**
     * Rewrites each pixel color from edited to match the closest color in p.
     * n must be > 1
     * if n != color_count, calls ReduceColors().
     * set colot_count to n
    */
    void AdaptToPaletteClosest(palette p, int n);

    /**
     * Saves edited image under name "filename" in current directory
    */
    void saveEdit(string filename);
    image(string filename);
    ~image();
};

QRgb match(QRgb base, palette p);