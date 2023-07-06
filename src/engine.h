#pragma once

#include <string>

#include <QtGui/qimage.h>

#include "octree.h"
#include "palette.h"

using namespace std;

enum algoType {
    MEDIAN_CUT,
    OCTREE,
};

enum matchType {
    BASIC,
    DI_FLOYD_STEINBERG,
    DI_NOISE,
    DI_JARVISJN,
    DI_ATKINSON,
    DI_SIERRA,
    DI_SIERRA_TWO_ROW,
    DI_SIERRA_LITE,
    DI_BAYER_4X4,
    DI_BAYER_8x8,
};

class engine
{
private:
    //number of colors in edited image, -1 if never edited
    int color_count;

    //palette in use
    palette ediPal;

    //algo used to generate palette in use and get us to color_count
    int usedalgotype;

    // ----------------------------------------- Color Reduction -------------------------------------
    //Extraction Functions using Median cut algorithm
    palette ExtractPaletteMEDIAN(int n);
    void ReduceColorsMEDIAN(int n, int matchType);

    palette ExtractPaletteOctree(int n);
    void ReduceColorsOctree(int n, int matchType);

    // ----------------------------------------- Color Matching -------------------------------------
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
    void AdaptToPaletteClosestBayer4x4(palette p);
    void AdaptToPaletteClosestBayer8x8(palette p);


public:
    //original loaded image
    QImage original;
    //copy of original for dithering manips
    QImage cpy;
    //edited image
    QImage edited;

    /**
     * extracts color palette from original image with n colors.
     * n must be > 1
     * if n == color_count && usedalgotype == algotype, returns ediPal
     * else, processes algo algotype on original to extract colors - does NOT edit edited, does NOT edit ediPal, nor algotype.
    */
    palette ExtractPalette(int n, int algotype);
    
    /**
     * Processes algo algotype on original image, writes edited as the output.
     * n must be > 1
     * changes color_count to n, rewrites ediPal.
    */
    void ReduceColors(int n, int algotype, int matchType);

    /**
     * Rewrites each pixel color from edited to match the closest color in p.
     * Method depends on the matchType - see enumeration above
    */
    void AdaptToPaletteClosest(palette p, int matchType);


    /**
     * Saves edited image under name "filename" in current directory
    */
    void saveEdit(string filename);

    /**
     * Load new image as original, in case the engine was initiated without a file (for interface)
    */
    void openImage(QImage img);
    engine();
    engine(string filename);
    engine(QImage img);
    ~engine();
    palette getPalette(){return ediPal;};
};

QRgb match(QRgb base, palette p);
