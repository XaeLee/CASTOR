#include "engine.h"
#include <iostream>

using namespace std;
using namespace mypalette;

/**
 * Creates an engine using an image filename, and opens it.
 * @param filename The path to the chosen image
 * @return Itself, the engine, with the image loaded
 */
engine::engine(string filename){
    this->original = QImage(filename.c_str());
    this->color_count = -1;
    this->edited = QImage(this->original.width(), this->original.height(), this->original.format());
}

/**
 * Creates an empty engine
 * @return Itself, an empty engine
 */
engine::engine(){
    this->color_count = 0;
}

/**
 * Creates an engine using and loading a QImage.
 * @param img The chosen image pre-loaded into a QImage.
 * @return Itself, the engine, with the image loaded
 */
engine::engine(QImage img){
    this->original = img;
    this->color_count = -1;
    this->edited = QImage(this->original.width(), this->original.height(), this->original.format());
}

engine::~engine(){

}
/**
 * Loads the given QImage as new original image
 * @param img The chosen image pre-loaded into a QImage
 * @return Nothing
 */
void engine::openImage(QImage img){
    this->original = img;
    this->color_count = -1;
    this->edited = QImage(this->original.width(), this->original.height(), this->original.format());
}

/**
 * Saves the edited image under a chosen name and location.
 * @param filename The name and path to save the image as
 * @return Nothing
 */
void engine::saveEdit(string filename){
    this->edited.save(filename.c_str());
}

/**
 * Wrapping function to redirect and call the appropriate palette
 *  extraction function given the chosen algorithm type.
 * @param n The number of colors to extract
 * @param algotype The chosen algorithm type
 * @return The extracted palette
 */
palette engine::ExtractPalette(int n, int algotype){
    switch (algotype)
    {
    case MEDIAN_CUT:
        return this->ExtractPaletteMEDIAN(n);
        break;
    case OCTREE:
        return this->ExtractPaletteOctree(n);
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

/**
 * Applies color reduction using chosen reduction algorithm and match type.
 * @param n The number of colors to reduce to
 * @param algotype The chosen reduction algorithm type
 * @param matchType The chosen matching algorithm type
 * @return Nothing
 */
void engine::ReduceColors(int n, int algotype, int matchType){
    switch (algotype)
    {
    case MEDIAN_CUT:
        this->ReduceColorsMEDIAN(n, matchType);
        this->usedalgotype = algotype;
        break;
    case OCTREE:
        this->ReduceColorsOctree(n, matchType);
        this->usedalgotype = algotype;
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

/**
 * Wrapping function to redirect and call the appropriate matching function
 * given the chosen matching algorithm.
 * @param p The palette to match the image to
 * @param matchType The chosen matching algorithm
 * @return Nothing
 */
void engine::AdaptToPaletteClosest(palette p, int matchType){
    switch (matchType)
    {
    case BASIC:
        this->AdaptToPaletteClosestNeutral(p);
        break;
    case DI_FLOYD_STEINBERG:
        this->AdaptToPaletteClosestFloyd_Steinberg(p);
        break;
    case DI_NOISE:
        this->AdaptToPaletteClosestNoise(p);
        break;
    case DI_JARVISJN:
        this->AdaptToPaletteClosestJarvis(p);
        break;
    case DI_ATKINSON:
        this->AdaptToPaletteClosestAtkinson(p);
        break;
    case DI_SIERRA:
        this->AdaptToPaletteClosestSierra(p);
        break;
    case DI_SIERRA_TWO_ROW:
        this->AdaptToPaletteClosestSierraTwoRows(p);
        break;
    case DI_SIERRA_LITE:
        this->AdaptToPaletteClosestSierraLite(p);
        break;
    case matchType::DI_BAYER_4X4:
        this->AdaptToPaletteClosestBayer4x4(p);
        break;
    case matchType::DI_BAYER_8x8:
        this->AdaptToPaletteClosestBayer8x8(p);
        break;
    default:
        cout << "please provide a valid algorithm type" << endl;
        break;
    }
}

/**
 * Wrapping function : builds an octree looking for n colors, and reduces
 * colors from the engine's loaded image - uses the chosen matching algorithm.
 * @param n The number of colors to reduce to
 * @param matchType The chosen matching algorithm
 * @return Nothing
 */
void engine::ReduceColorsOctree(int n, int matchType){
    octree t(n);
    this->ediPal = t.reduceColors(this->original);
    cout << "done with reducting" << endl;
    this->AdaptToPaletteClosest(ediPal, matchType);    
}

/**
 * Wrapping function : builds an octree and gets reduced color palette of n
 * colors from the engine's loaded image.
 * @param n The number of colors to get for the palette
 * @return A palette with the n most used colors in the engine's loaded image.
 */
palette engine::ExtractPaletteOctree(int n){
    octree t(n);
    return t.reduceColors(this->original);
}