#pragma once

#include <string>

#include <qt5/QtGui/qimage.h>

#include "palette.h"

using namespace std;
enum algoType {MEDIAN_CUT,};
enum matchType {BASIC, DI_FLOYD_STEINBERG, DI_NOISE, DI_JARVISJN, DI_ATKINSON, DI_SIERRA, DI_SIERRA_TWO_ROW, DI_SIERRA_LITE};

class engine
{
private:
    //original loaded image
    QImage original;
    //copy of original for dithering manips
    QImage cpy;
    //edited image
    QImage edited;
    //number of colors in edited image, -1 if never edited
    int color_count;
    //
    int usedAlgo;

    //palette in use
    palette ediPal;

    //Color Reduction / Extraction Functions using Median cut algorithm
    palette ExtractPaletteMEDIAN(int n);
    void ReduceColorsMEDIAN(int n);

    // Simple matching from edited image to palette
    void AdaptToPaletteClosestNeutral(palette p);
    //Dithering algos
    void AdaptToPaletteClosestFloyd_Steinberg(palette p);
    void AdaptToPaletteClosestNoise(palette p);
    void AdaptToPaletteClosestJarvis(palette p);
    void AdaptToPaletteClosestAtkinson(palette p);
    void AdaptToPaletteClosestSierra(palette p);
    void AdaptToPaletteClosestSierraTwoRows(palette p);
    void AdaptToPaletteClosestSierraLite(palette p);


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
     * Method depends on the matchType - see enumeration above
    */
    void AdaptToPaletteClosest(palette p, int matchType);


    /**
     * Saves edited image under name "filename" in current directory
    */
    void saveEdit(string filename);
    engine(string filename);
    ~engine();
};

QRgb match(QRgb base, palette p);