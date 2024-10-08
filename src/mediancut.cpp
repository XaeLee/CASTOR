#include "engine.h"
#include "colorMatching.h"

/**
 * Extracts a n colors palette from the engine's loaded image, using median cut algorithm.
 * @param n The number of color to keep for the palette
 * @return The built n colors palette
 */
palette engine::ExtractPaletteMEDIAN(int n){
    if (n < 2)
        throw std::invalid_argument("n must be > 1 for color reduction");
    if (n == this->color_count){
        return this->ediPal;
    }
    palette res;

    int lr = qRed(this->original.pixel(0, 0));
    int lg = qGreen(this->original.pixel(0, 0));
    int lb = qBlue(this->original.pixel(0, 0));

    int ur = 0;
    int ug = 0;
    int ub = 0;

    int nbCols = this->original.width();
    int nbRows = this->original.height();

    vector<QRgb> seenColors;

    /* Extracts highest and smallest values in R, G and B channels of the image to adapt the reduction to
    the biggest range.
    */
    for (int col = 0; col < nbCols; col++){
        for (int row = 0; row < nbRows; row++){
            QRgb currpix = this->original.pixel(col, row);
            if (find(seenColors.begin(), seenColors.end(), currpix) == seenColors.end()){
                lr = min(lr, qRed(currpix));
                lg = min(lg, qGreen(currpix));
                lb = min(lb, qBlue(currpix));

                ur = max(ur, qRed(currpix));
                ug = max(ug, qGreen(currpix));
                ub = max(ub, qBlue(currpix));
                seenColors.push_back(currpix);
            }
        }
    }
    int red_range = ur - lr;
    int green_range = ug - lg;
    int blue_range = ub - lb;
    int max_range = max(max(red_range, green_range), blue_range);

    qSort(seenColors.begin(), seenColors.end(), [max_range, red_range, green_range](const QRgb& rgb1, const QRgb& rgb2){
        //tried using a switch instead of ugly ifs but can't since ranges are not constant
        if (max_range == red_range)
            return qRed(rgb1) < qRed(rgb2);
        if (max_range == green_range)
            return qGreen(rgb1) < qGreen(rgb2);
        return qBlue(rgb1) < qBlue(rgb2);
    });

    vector<vector<QRgb>> lists;
    int list_size = seenColors.size()/n;
    for (int i = 0; i< n; i++){
        vector<QRgb> curr;
        for (int j = list_size * i; j <list_size * (i + 1); j++){
            curr.push_back(seenColors.at(j));
        }
        lists.push_back(curr);
    }
    
    for (vector<QRgb> curr: lists){
        res.addColor(curr.at(curr.size()/2));
    }
    return res;
}

/**
 * Builds a n-colors palette from the engine's loaded image, using median cut algorithme,
 * and matches all pixel colors from the original image to a new one from said palette.
 * Saves the results in the engine's edited image.
 * @param n The number of colors to reduce to
 * @param matchType The chosen matching algorithm chosen to match the original image's
 * pixels to the reduced palette
 * @return Nothing
 */
void engine::ReduceColorsMEDIAN(int n, int matchtype){
    if (n < 2)
        throw std::invalid_argument("n must be > 1 for color reduction");
    palette res;

    int lr = qRed(this->original.pixel(0, 0));
    int lg = qGreen(this->original.pixel(0, 0));
    int lb = qBlue(this->original.pixel(0, 0));

    int ur = 0;
    int ug = 0;
    int ub = 0;

    int nbCols = this->original.width();
    int nbRows = this->original.height();

    vector<QRgb> seenColors;

    /* Extracts highest and smallest values in R, G and B channels of the image to adapt the reduction to
    the biggest range.
    */
    for (int col = 0; col < nbCols; col++){
        for (int row = 0; row < nbRows; row++){
            QRgb currpix = this->original.pixel(col, row);
            if (find(seenColors.begin(), seenColors.end(), currpix) == seenColors.end()){
                lr = min(lr, qRed(currpix));
                lg = min(lg, qGreen(currpix));
                lb = min(lb, qBlue(currpix));

                ur = max(ur, qRed(currpix));
                ug = max(ug, qGreen(currpix));
                ub = max(ub, qBlue(currpix));
                seenColors.push_back(currpix);
            }
        }
    }
    int red_range = ur - lr;
    int green_range = ug - lg;
    int blue_range = ub - lb;
    int max_range = max(max(red_range, green_range), blue_range);

    qSort(seenColors.begin(), seenColors.end(), [max_range, red_range, green_range](const QRgb& rgb1, const QRgb& rgb2){
        //tried using a switch instead of ugly ifs but can't since ranges are not constant
        if (max_range == red_range)
            return qRed(rgb1) < qRed(rgb2);
        if (max_range == green_range)
            return qGreen(rgb1) < qGreen(rgb2);
        return qBlue(rgb1) < qBlue(rgb2);
    });

    vector<vector<QRgb>> lists;
    int list_size = seenColors.size()/n;
    for (int i = 0; i< n; i++){
        vector<QRgb> curr;
        for (int j = list_size * i; j <list_size * (i + 1); j++){
            curr.push_back(seenColors.at(j));
        }
        lists.push_back(curr);
    }
    
    for (vector<QRgb> curr: lists){
        res.addColor(curr.at(curr.size()/2));
    }
    
    this->ediPal = res;
    this->color_count = n;

    this->AdaptToPaletteClosest(ediPal, matchtype);
}
