#include "image.h"
#include <iostream>

using namespace std;

int color_distance(QRgb p1, QRgb p2){
    int r1 = qRed(p1);
    int g1 = qGreen(p1);
    int b1 = qBlue(p1);

    int r2 = qRed(p2);
    int g2 = qGreen(p2);
    int b2 = qBlue(p2);

    return abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2);
}

QRgb match(QRgb base, palette p){
    int index = 0;
    int current_distance = INT_MAX;
    vector<QRgb> cols = p.getColors();
    for (int i = 0; i<cols.size(); i++){
        int dist = color_distance(base, cols.at(i));
        if (dist < current_distance){
            current_distance = dist;
            index = i;
        }
    }
    return cols[index];
}

image::image(string filename){
    this->original = QImage(filename.c_str());
    this->color_count = -1;
}

image::~image(){

}

palette image::ExtractPalette(int n){
    if (n < 2)
        throw std::invalid_argument("n must be > 1 for color reduction");
    if (n == this->color_count){
        return this->origiPal;
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

void image::ReduceColors(int n){
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
    
    this->origiPal = res;

    QImage out(nbCols, nbRows, QImage::Format_ARGB32);
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbCols; y++){
            QRgb m = match(original.pixel(x, y), res);
            out.setPixel(x, y, m);
        }
    }

    this->edited = out;
    this->color_count = n;
}

void image::AdaptToPaletteClosest(palette p, int n){
    if (n < 2)
        throw std::invalid_argument("n must be > 1 for color reduction");
    if (n != this->color_count)
        this->ReduceColors(n);
    int nbCols = this->original.width();
    int nbRows = this->original.height();
    
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbCols; y++){
            QRgb m = match(original.pixel(x, y), p);
            this->edited.setPixel(x, y, m);
        }
    }
}

void image::saveEdit(string filename){
    this->edited.save(filename.c_str());
}