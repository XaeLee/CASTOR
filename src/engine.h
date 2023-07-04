#pragma once

#include <string>

#include <qt5/QtGui/qimage.h>

#include "palette.h"

using namespace std;
enum {MEDIAN_CUT, };
class engine
{
private:
    //original loaded image
    QImage original;
    //edited image
    QImage edited;
    //number of colors in edited image, -1 if never edited
    int color_count;
    //
    int usedAlgo;

    //palette in use
    palette ediPal;


    palette ExtractPaletteMEDIAN(int n);
    void ReduceColorsMEDIAN(int n);
    void AdaptToPaletteClosestMEDIAN(palette p);

public:
    /**
     * extracts color palette from original image with n colors.
     * n must be > 1
     * if n == color_count, returns ediPal
     * else, processes algo algotype on original to extract colors - does NOT edit edited, does NOT edit ediPal.
    */
    palette ExtractPalette(int n, int algotype);
    
    /**
     * Processes algo algotype on original image, writes edited as the output.
     * n must be > 1
     * changes color_count to n, rewrites ediPal.
    */
    void ReduceColors(int n, int algotype);

    /**
     * Rewrites each pixel color from edited to match the closest color in p.
    */
    void AdaptToPaletteClosest(palette p, int algotype);

    /**
     * Saves edited image under name "filename" in current directory
    */
    void saveEdit(string filename);
    engine(string filename);
    ~engine();
};

QRgb match(QRgb base, palette p);